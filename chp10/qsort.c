/*   1  */  typedef struct {
/*   2  */      char *data;
/*   3  */      int key;
/*   4  */  } item;
/*   5  */  
/*   6  */  item array[] = {
/*   7  */      {"bill", 3},
/*   8  */      {"neil", 4},
/*   9  */      {"john", 2},
/*  10  */      {"rick", 5},
/*  11  */      {"alex", 1},
/*  12  */  };
/*  13  */  
/*  14  */  sort(a,n)
/*  15  */  item *a; 
/*  16  */  {
                int i, j;
                for (i = 0; i < n; i ++) {
                    for (j = 1; j < n - i; j ++) {
                        if (a[j - 1].key > a[j].key) {
                            item temp = a[j - 1];
                            a[j - 1] = a[j];
                            a[j] = temp;
                        }
                    }
                }
/*  32  */  }
/*  33  */  
/*  34  */  main()
/*  35  */  {
/*  36  */      sort(array,5);
                int i;
                for(i = 0; i < 5; i++)
/*  39  */          printf("array[%d] = {%s, %d}\n",
/*  40  */              i, array[i].data, array[i].key);
/*  37  */  }
