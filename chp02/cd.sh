#!/bin/bash

# 定义全局变量
menu_choice=""
current_cd=""
title_file="title.cdb"
tracks_file="tracks.cdb"
temp_file=/tmp/cdb.$$                  #
trap 'rm -f $temp_file' EXIT           #

###################################################################
# echo -e 处理特殊字符:
#
# \a 发出警告声
# \b 删除前一个字符
# \c 最后不加上换行符号
# \f 换行但光标仍旧停留在原来的位置
# \n 换行且光标移至行首
# \r 光标移至行首，但不换行
# \t 插入tab
# \v 与\f相同
# \ 插入\字符
# \nnn 插入nnn（八进制）所代表的ASCII字符
get_return() {
    echo -e "Press return \c"          # echo -e 处理特殊字符
    read x                             # read x 将用户的输入赋值给变量x
    return 0
}

get_confirm() {
    echo -e "Are you sure? \c"
    while true
    do
        read x
        case "$x" in
            y | yes | Y | Yes | YES )
                return 0;;
            n | no | N | No | NO )
                echo
                echo "Cancelled"
                return 1;;
            *) echo "Please enter yes or no" ;;
        esac
    done
}

set_menu_choice() {
    clear
    echo "Options :-"
    echo
    echo "    a) Add new CD"
    echo "    f) Find CD"
    echo "    c) Count the CDs and tracks in the catalog"
    if [ "$cdcatnum" != "" ]; then
        echo "    l) List tracks on $cdtitle"
        echo "    r) Remove $cdtitle"
        echo "    u) Update track information fo $cdtitle"
    fi
    echo "    q) Quit"
    echo
    echo -e "Please enter choice then press return \c"
    read menu_choice
    return
}

insert_title() {
    echo $* >> $title_file             #
    return
}

insert_track() {
    echo $* >> $tracks_file
    return
}

add_record_tracks() {
    echo "Enter track information for this CD"
    echo "When no more tracks enter q"
    cdtrack=1
    cdtitle=""
    while [ "$cdtitle" != "q" ]
    do
        echo -e "Track $cdtrack, track title? \c"
        read tmp
        cdtitle=${tmp%%,*}             #
        if [ "$tmp" != "$cdtitle" ]; then
            echo "Sorry, no commas allowed"
            continue
        fi
        if [ -n "$cdtitle" ]; then
            if [ "$cdtitle" != "q" ]; then
                insert_track $cdcatnum,$cdtrack,$cdtitle
            fi
        else
            cdtrack=$((cdtrack-1))
        fi
        cdtrack=$((cdtrack+1))
    done    
}

add_records() {
    echo -e "Enter catalog name \c"
    read tmp
    cdcatnum=${tmp%%,*}

    echo -e "Enter title \c"
    read tmp
    cdtitle=${tmp%%,*}

    echo -e "Enter type \c"
    read tmp
    cdtype=${tmp%%,*}

    echo -e "Enter artist/composer \c"
    read tmp
    cdac=${tmp%%,*}

    echo About to add new entry
    echo "$cdcatnum $cdtitle $cdtype $cdac"

    if get_confirm; then
        insert_title $cdcatnum,$cdtitle,$cdtype,$cdac
        add_record_tracks
    else
        remove_recodes
    fi

    return
}

find_cd() {
    if [ "$1" = "n" ]; then
        asklist=n
    else
        asklist=y
    fi
    echo $asklist
    cdcatnum=""
    echo -e "Enter a string to search for in the CD titles \c"
    read searchstr
    if [ "$searchstr" = "" ]; then
        return 0
    fi

    # grep(General Regular Expression Parser) 在文件中搜索字符串
    # 语法: grep [options] PATTERN [FILES]
    # options: -c -E -h -i -l -v
    grep "$searchstr" $title_file > $temp_file
    
    set $(wc -l $temp_file)            #
    linesfound=$1
    

    case "$linesfound" in
        0) echo "Sorry, nothing found"
           get_return
           return 0
           ;;
        1) ;;
        2) echo "Sorry, not unique."
           echo "Found the following"
           cat $temp_file
           get_return
           return 0
    esac

    IFS=","                            #
    read cdcatnum cdtitle cdtype cdac < $temp_file
    IFS=" "

    if [ -z "$cdcatnum" ]; then        # -z string 如果字符串为 null 则为真
        echo "Sorry, could not extract catalog field from $temp_file"
        get_return
        return 0
    fi

    echo
    echo Catalog number: $cdcatnum
    echo Title: $cdtitle
    echo Type: $cdtype
    echo Artist/Composer: $cdac
    echo
    get_return

    if [ "$asklist" = "y" ]; then
        echo -e "View tracks for this CD? \c"
            read x
        if [ "$x" = "y" ]; then
            echo
            list_tracks
            echo
        fi
    fi
    return 1
}

update_cd() {
    if [ -z "$cdcatnum" ]; then
        echo "Current tracks are :-"
        list_tracks
        echo
        echo "This will re-enter the tracks for $cdtitle"
        get_confirm && {               #
            grep -v "^${cdcatnum}," $tracks_file > $temp_file
            mv $temp_file $tracks_file
            echo
            add_record_tracks
        }
    fi
    return
}

count_cds() {
    echo en
    set $(wc -l $title_file)           # 把 wc -l $title_file 命令的输出设置为参数列表
    num_titles=$1                      # 通过位置参数 $1 获得行数
    set $(wc -l $tracks_file)
    num_tracks=$1
    echo found $num_titles CDs, with a total of $num_tracks tracks
    get_return
    return
}

#########################################
# 注意:grep -v "^cdcatnum" > $title_file
# $title_file文件就会在grep命令开始执行之前,
# 被>输出重定向操作设置为空文件,结果导致grep命
# 令将从一个空文件里读取数据.
#########################################
remove_recodes() {
    if [ -z "$cdcatnum" ]; then
        echo "You must select a CD first"
        find_cd n
    fi
    if [ -n "$cdcatnum" ]; then
        echo "You are about to delete $cdtitle"
        get_confirm && {
            grep -v "^${cdcatnum}," $title_file > $temp_file
            mv $temp_file $title_file
            grep -v "^${cdcatnum}," $tracks_file > $temp_file
            mv $temp_file $track_file
            cdcatnum=""
            echo "Entry removed"
        }
        get_return
    fi
    return
}

list_tracks() {
    if [ "$cdcatnum" = "" ]; then
        echo "no CD selected yet"
        return
    else
        grep "^${cdcatnum}," $tracks_file > $temp_file
        num_tracks=$(wc -l $temp_file)
        if [ "$num_tracks" = "0" ]; then
            echo "no tracks found for $cdtitle"
        else {
            echo
            echo "$cdtitle :-"
            echo
            cut -f 2- -d , $temp_file
            echo
        } | ${PAGER:-more}
        fi
    fi
    get_return
    return
}

rm -f $temp_file
if [ ! -f $title_file ]; then
    touch $title_file
fi
if [ ! -f $track_file ]; then
    touch $track_file
fi

clear
echo
echo
echo "Mini CD manager"
sleep 1

quit=n
while [ "$quit" != "y" ];
do
    set_menu_choice
    case "$menu_choice" in
        a) add_records;;
        r) remove_recodes;;
        f) find_cd y;;
        u) update_cd;;
        c) count_cds;;
        l) list_tracks;;
        b)
           echo
           more $title_file
           echo
           get_return;;
        q | Q ) quit=y;;
        *) echo "Sorry, choice not recognized";;
    esac
done

rm -f $temp_file
echo "Finished"
exit 0