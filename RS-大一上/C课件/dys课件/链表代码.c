struct STU{
    char name[8];
    float cor;
    struct STU *pNext;
};
void main(){
    STU *pHdr=NULL,*pCur=NULL;
    int i,n;    
    // 新建链表
    scanf("%d",&n );
    for( i=0;i<n;i++ ){
        if ( i==0 ){
            pCur = pHdr = new STU;
            pCur->pNext = NULL;
        }else{
            pCur->pNext = new STU;
            pCur = pCur->pNext;
            pCur->pNext = NULL;
        }
        scanf("%s%f",pCur->name,&pCur->cor);
    }
    // 遍历链表
    pCur = pHdr;
    while( pCur->pNext ){
        printf("%s %f\n",pCur->name,pCur->cor );
        pCur = pCur->pNext;
    }
    // 释放链表
    pCur = pHdr;
    while( pCur ){
        STU *pDel = pCur;
        pCur = pCur->pNext;
        delete pDel;
    }
}
