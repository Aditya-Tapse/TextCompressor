#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char filename[100];

char *data;
int len = 0;

char *(codes)[40];
char *character;
int *freq;
int diffCharacters = 0;

char *findata;
char *encodedtree;

int extratree = 0;
int extradata = 0;

char tempCode[100];
int iindex = 0;

struct Node
{
    char data;
    int freq ;
    struct Node *left;
    struct Node *right;
};

struct MinHeap
{
    int size;
    struct Node **array;
};

struct Node *root;

int scanData();
void createFreqArray();
void EncodeData();
int DecodeData();
void CreateTree1();
int CreateTree2();
struct Node *createNode(struct Node *left, struct Node *right, int freq, int data);
void Heapify(struct MinHeap *minHeap, int idx);
struct Node *GetMin(struct MinHeap *minHeap);
void insertNode(struct MinHeap *minHeap, struct Node *node);
struct MinHeap *createMinHeap();
void createCodes(struct Node *root);
void Huffman();

int main()
{
    int operation = 0;
    printf("1. Compress File\n2. Decompress File\n3.Exit\nEnter your choice : ");

    while (1)
    {
        scanf("%d",&operation);
        if (operation == 1 || operation == 2 || operation == 3)
        {
            break;
        }
        else
        {
            printf("Enter proper choice : ");
        }
    }
    if (operation == 1)
    {
        int isfileopened = scanData();
        if (isfileopened == 1)
        {
            createFreqArray();
            Huffman();
            EncodeData();
            CreateTree1();
            printf("File compression Succesful");
        }
    }
    else if (operation == 2)
    {
        int isok = CreateTree2();
        if (isok)
            isok = DecodeData();
        if (isok == 1)
        {
            printf("Data Decompressed Successfully");
        }
        else
        {
            printf("Can't Decompress file because of some error");
        }
    }
    return 0;
}

struct Node *createNode(struct Node *left, struct Node *right, int freq, int data)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    temp->left = left;
    temp->right = right;
    temp->freq = freq;
    temp->data = data;
    return temp;
}
void Heapify(struct MinHeap *minHeap, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != i)
    {
        struct Node *ta = minHeap->array[smallest];
        minHeap->array[smallest] = minHeap->array[i];
        minHeap->array[i] = ta;
        Heapify(minHeap, smallest);
    }
}
struct Node *GetMin(struct MinHeap *minHeap)
{
    struct Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    Heapify(minHeap, 0);
    return temp;
}

void insertNode(struct MinHeap *minHeap, struct Node *node)
{
    minHeap->size++;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

struct MinHeap *createMinHeap()
{
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->array = (struct Node **)malloc(diffCharacters * sizeof(struct Node *));
    for (int i = 0; i < diffCharacters; ++i)
    {
        struct Node *node = createNode(NULL, NULL, freq[i], character[i]);
        insertNode(minHeap, node);
    }
    return minHeap;
}

void createCodes(struct Node *root)
{
    if (root->left)
    {
        tempCode[iindex++] = '0';
        createCodes(root->left);
        iindex--;
    }
    if (root->right)
    {
        tempCode[iindex++] = '1';
        createCodes(root->right);
        iindex--;
    }
    if (!root->left && !root->right)
    {
        int ind = 0;
        for (int i = 0; i < diffCharacters; i++)
        {
            if (character[i] == root->data)
            {
                ind = i;
                break;
            }
        }
        for (int i = 0; i < iindex; i++)
        {
            codes[ind][i] = tempCode[i];
        }
        codes[ind][iindex] = '\0';
    }
}

void Huffman()
{
    struct MinHeap *minHeap = createMinHeap();
    while (minHeap->size > 1)
    {
        struct Node *left = GetMin(minHeap);
        struct Node *right = GetMin(minHeap);
        struct Node *main = createNode(NULL, NULL, left->freq + right->freq, '?');
        main->left = left;
        main->right = right;
        insertNode(minHeap, main);
    }
    root = GetMin(minHeap);
    iindex = 0;
    createCodes(root);
    for (int i = 0; i < diffCharacters; i++)
    {
        for (int j = 0; codes[i][j]; j++)
        {
            if (codes[i][j] == '0' || codes[i][j] == '1')
                continue;
            else
            {
                codes[i][j] = '\0';
                break;
            }
        }
    }
}

char go(int *cur)
{
    struct Node *temp = root;
    while (temp->left && temp->right)
    {
        if (findata[*cur] == '1')
        {
            temp = temp->right;
        }
        else
        { 
            temp = temp->left;
        }
        (*cur)++;
    }
    return temp->data;
}

struct Node *DecodeTree()
{
    if (iindex >= strlen(encodedtree))
        return NULL;
    if (encodedtree[iindex] == '1')
    {
        iindex++;
        int val = 0;
        for (int cnt = 1; cnt <= 8 && iindex < strlen(encodedtree); cnt++, iindex++)
        {
            if (encodedtree[iindex] == '1')
                val += (1 << (8 - cnt));
        }
        return createNode(NULL, NULL, 0, (char)val);
    }
    else
    {
        iindex++;
        struct Node *leftNode = DecodeTree();
        struct Node *rightNode = DecodeTree();
        return createNode(leftNode, rightNode, 0, '?');
    }
}

int CreateTree2()
{
    printf("Enter the file name where encodeded data is stored: ");
    scanf("%s" ,filename);
    FILE *FIN = fopen(filename, "r");
    if (FIN == NULL)
        return 0;
    char dataInText[1000];
    encodedtree = (char *)malloc(15000 * sizeof(char));
    strcpy(encodedtree, "");
    char c;
    iindex = 0;
    int sz = 0;
    for (int i = 0, power = 1000; i < 4; i++, power /= 10)
    {
        sz += (fgetc(FIN) - '0') * (power);
    }
    int i = 8;

    while (iindex < sz)
    {
        fseek(FIN, i, SEEK_SET);
        c = fgetc(FIN);
        if (c == -1)
            c = 26;
        dataInText[iindex] = c;
        iindex++;
        i++;
        if (c == '\n')
            i++;
    }
    dataInText[iindex] = '\0';
    for (int i = 0; i < sz; i++)
    {
        char bin[8];
        int d = dataInText[i];
        for (int j = 0; j < 7; j++)
        {
            bin[6-j] = (d % 2) + '0';
            d /= 2;
        }
        bin[7] = '\0';
        strcat(encodedtree, bin);
    }
    fseek(FIN, -2, SEEK_END);
    int extraTree = fgetc(FIN) - '0';
    encodedtree[strlen(encodedtree) - extratree] = '\0';
    fclose(FIN);
    iindex = 0;
    root = DecodeTree();
    return 1;
}

int DecodeData()
{
    FILE *FIN = fopen(filename, "r");
    if (FIN == NULL)
        return 0;
    FILE *FOP = fopen("decompressed.txt", "w");
    findata = (char *)malloc(15000 * sizeof(char));
    strcpy(findata, "");
    char c;
    int sz1 = 0, sz2 = 0;
    for (int i = 0, power = 1000; i < 4; i++, power /= 10)
    {
        sz1 += (fgetc(FIN) - '0') * (power);
    }
    for (int i = 0, power = 1000; i < 4; i++, power /= 10)
    {
        sz2 + (fgetc(FIN) - '0') * (power);
    }
    int numberOfNewLine = 0;
    int i = 8, j = 0;
    iindex = 0;
    while (iindex < sz1)
    {
        fseek(FIN, i, SEEK_SET);
        c = fgetc(FIN);
        iindex++;
        i++;
        if (c == '\n')
        {
            i++;
            numberOfNewLine++;
        }
    }
    i = 8 + sz1 + numberOfNewLine;
    while (j < sz2)
    {
        fseek(FIN, 1, SEEK_SET);
        c - fgetc(FIN);
        if (c == -1)
            c = 26;
        char bin[8];
        int d = c;
        for (int j = 0; j < 7; j++)
        {
            bin[6 - j] = (d % 2) + '0';
            d /= 2;
        }
        bin[7] == '\0';
        strcat(findata, bin);
        i++;
        j++;
        if (c == '\n')
            i++;
    }
    fseek(FIN, -1, SEEK_END);
    int extraData = fgetc(FIN) - '0';
    findata[strlen(findata) - extradata] = '\0';
    for (int i = 0; i < strlen(findata);)
    {
        c = go(&i);
        fprintf(FOP, "%c", c);
    }
    fclose(FOP);
    fclose(FIN);
    return 1;
}

void EncodeTree(struct Node *node)
{
    if (node->left == NULL && node->right == NULL)
    {
        strcat(encodedtree, "1");
        int val = node->data;
        char bin[9] = "00000000";
        int cur = 7;
        while (val > 0)
        {
            bin[cur] = (char)((val % 2) + '0');
            val /= 2;
            cur--;
        }
        strcat(encodedtree, bin);
    }
    else
    {
        strcat(encodedtree, "0");
        EncodeTree(node->left);
        EncodeTree(node->right);
    }
}

void CreateTree1()
{
    encodedtree = (char *)malloc(15000 * sizeof(char));
    strcpy(encodedtree, "");
    EncodeTree(root);
    char dataInText[15000];
    int sz = 0;
    for (int i = 0; i < strlen(encodedtree);)
    {
        char bin[8];
        int cur = 0;
        if (strlen(encodedtree) - i < 7)
            extratree = 7 - (strlen(encodedtree) - i);
        for (int cnt = 6; cnt >= 0; cnt--, i++)
        {
            cur += (encodedtree[i] == '1') * (1LL << (cnt));
        }
        dataInText[sz] = (char)cur;
        sz++;
    }
    dataInText[sz] = '\0';
    FILE *FOP = fopen("compressed. txt", "w");
    fprintf(FOP, "%04d%04d", sz, iindex);
    for (int i = 0; i < sz; i++)
    {
        fprintf(FOP, "%c", dataInText[i]);
    }
    for (int i = 0; i < iindex; i++)
    {
        fprintf(FOP, "%c", findata[i]);
    }
    fprintf(FOP, "%d%d", extratree, extradata);
    fclose(FOP);
}

void EncodeData()
{
    FILE *FIN = fopen(filename, "r");
    char c, dataInBin[15000];
    findata = (char *)malloc(15000* sizeof(char));
    strcpy(findata, "");
    while ((c = fgetc(FIN)) != EOF)
    {
        for (int i = 0; i < diffCharacters; i++)
        {
            if (character[i] == c)
            {
                strcat(dataInBin, codes[i]);
                break;
            }
        }
    }
    iindex = 0;
    for (int i = 0; i < strlen(dataInBin);)
    {
        int cur =0;
        if (strlen(dataInBin)- i < 7)
            extradata = 7- (strlen(dataInBin) - i);
        for (int cnt = 6; cnt >= 0 && i < strlen(dataInBin); i++, cnt--)
        {
            cur += (1 << (cnt)) * (dataInBin[i] == '1');
        }
        findata[iindex] = (char)cur;
        iindex++;
    }
    findata[iindex] = '\0';
    fclose(FIN);
}

int scanData()
{
    printf("Enter the file name: ");
    scanf("%s", filename);
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("\nThere is no file with given name:");
        return 0;
    }
    else
    {
        printf("\nFile opened successfully:)\n");
    }
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);

    data=(char *) malloc((len + 5) * sizeof(char));
    char c;
    int curIndex = 0;
    fseek(fp, 0, SEEK_SET);
    while ((c = fgetc(fp)) != EOF)
    {
        data[curIndex++] = c;
    }
    data[curIndex] = '\0';
    return 1;
}

void createFreqArray()
{
    int cnt[128] = {};
    for (int i = 0; i < len && data[i]; i++)
    {
        cnt[data[i]]++;
    }
    for (int i = 0; i < 128; i++)
    {
        if (cnt[i] >= 1)
        {
            ++diffCharacters;
        }
    }
    character = (char *) malloc(diffCharacters * sizeof(char));
    freq=(int *) malloc(diffCharacters *sizeof(int));
    * codes = malloc(sizeof(char[diffCharacters][40]));
    for (int i = 0, j = 0; i < 128; i++)
    {
        if (cnt[i] >= 1)
        {
            character[j] = (char)(i);
            freq[j++] = cnt[i];
        }
        for (int i = 0; i < diffCharacters - 1; i++)
        {
            for (int j = 0; j < diffCharacters - i - 1; j++)
            {
                if (freq[j] > freq[j + 1])
                {
                    int t1 = freq[j];
                    freq[j] = freq[j + 1];
                    freq[j + 1]= t1;
                    char t2 = character[j];
                    character[j] = character[j + 1];
                    character[j + 1] = t2;
                }
            }
        }
    }
}