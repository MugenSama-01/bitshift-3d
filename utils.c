int len(const char *str){
    int l=0;
    while (str[l]!= 0)
        l++;
    return l;
}

void print(const char *str){

    __asm__ volatile(
        "syscall"
        :
        :"a" (1),
        "D" (1),
        "S" (str),
        "d" (len(str))
        :"rcx","r11","memory"
);
}

void printn(int num){
    int i = 0;
    char l[20];
    int is_neg = 0;

    if (num < 0){
        is_neg = 1;
        num = -num;
    }

    if (num==0){
        l[i]='0';
        i++;
    }
    else{
        while (num>0){
            l[i] = num%10 + '0';
            num = num/10;
            i++;
        }
    }

    char output[20];
    int j = 0;

    if (is_neg){
        output[j]= '-';
        j++;
    }

    while (i>0){
        i--;
        output[j]=l[i];
        j++;
    }

    output[j]='\n';
    j++;
    output[j]='\0';

    print(output);
}