#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Structures
typedef struct node_t node;
struct node_t{
    char secret[256];
    int length;
    int byte_index;
    int bit_position;
    int flag;
};

//Private Functions
void InitializeNode(node *data, char *secret){
    strcpy(data->secret, secret);
    data->length = strlen(secret);
    data->byte_index = 0;
    data->bit_position = 0;
    data->flag = 1;
}

int power(int x, int y){
    int pow = 1;
    if(y == 0)
        return pow;
    while(y--)
        pow= pow*x;
    return pow;
}

int readBit(node *data){
    char temp = data->secret[data->byte_index];
    temp = temp>>data->bit_position;

    if(data->bit_position == 7){
        data->bit_position = 0;
        if(data->byte_index == data->length -1)
            data->flag = 0;
        else
            data->byte_index++;
    }
    else
    data->bit_position++;

    if(temp&1 == 1)
       return 1;
    else
       return 0;
}

void writeBit(char *buf, int bit){
    *buf = *buf >> 1;
    *buf = *buf << 1;
    *buf = *buf + bit;
}

int readLastBit(char *ptr){
    if(*ptr&1 == 1)
        return 1;
    return 0;
}

void writeByte(node *data, int byte){
    data->secret[data->byte_index++] = byte;
}

void changeBuffer(char *buf, int count, node *data){
    int index = 100, bit = 0, n;
    int interval = count/(256*8);
    for(n=0;n<data->length*8&&data->flag;index+=interval,n++){
        bit = readBit(data);
        writeBit(&buf[index],bit);
    }
    for(n=0;n<8;index+=interval,n++){
        writeBit(&buf[index],0);
    }
}

void getSecret(char *buf, int count, node *data){
    int index = 100, bit = 0, i = 0, byte = 0;
    int interval = count/(256*8);
    for(;;index+=interval){
        bit = readLastBit(&buf[index]);
        if(i<7){
            byte = byte + bit*power(2,i++);
        }
        else{
            i = 0;
            writeByte(data,byte);
            if(byte == 0)
                break;
            else
                byte = 0;
        }
    }
}

//Public Functions
char* inscribe(char *InputStream, char *secret, int count){
    node data;
    InitializeNode(&data, secret);
    changeBuffer(InputStream, count, &data);
    return InputStream;
}

char* extract(char *InputStream, int count){
    node *data = (node*)malloc(sizeof(node));
    char secret[256];
    InitializeNode(data, "");
    getSecret(InputStream, count, data);
    strcpy(secret, data->secret);
    return secret;
}
