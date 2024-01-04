#include<bits/stdc++.h>
using namespace std;

char str[100];

int main() {
    printf("test\n");

    int i=0, cnt;
    scanf("%x", &cnt);
    while(i<cnt && scanf(" 0x") != EOF) {
        int c=0;
        for(int j=0; j<4; j++) {
            scanf("%2x", &c);
            str[i++] = c;
        }
    }

    for(int i=0; i<cnt; i+=4) {
        swap(str[i], str[i+3]);
        swap(str[i+1], str[i+2]);
    }
    
    for (int i = 0; i < cnt; i++)
    {
        cout<<str[i];
    }
    cout<<endl;
    
}