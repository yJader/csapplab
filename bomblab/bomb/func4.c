int func4 (int edi, int esi, int edx) // edi=a1, esi=0, edx=0xe
{
    // temporaries
    int eax, ebx;

    eax = edx;
    eax = eax - esi; 
    ebx = eax;          // ebx = eax = edx - esi;
    ebx = (unsigned int)ebx >> 31; //符号位
    eax = eax + ebx;
    eax = eax >> 1;
    ebx = eax + esi + 1;
    if (ebx <= edi) goto L1; 
    edx = ebx - 1;
    eax = func4 (edi, esi, edx);
    ebx = ebx + eax;
    goto L2;
L1:
    if (ebx >= edi) goto L2;
    esi = ebx + 1;
    eax = func4 (edi, esi, edx);
    ebx = ebx + eax;
L2:
    eax = ebx;
    return eax;
}