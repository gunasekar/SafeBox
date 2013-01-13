/*
 * File:   main.c
 * Author: root
 *
 * Created on January 30, 2011, 8:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "steganography.h"

long GetFileSize( FILE *f ){
    long pos, size;
    pos = ftell( f );
    fseek( f, 0, SEEK_END );
    size = ftell( f );
    fseek( f, pos, SEEK_SET );
    return size;
}

int readSecret(char* secret){
    int i;
    char message[256];
    printf("\nEnter the no. of words in the secret:");
    scanf("%d",&i);
    printf("\nEnter the Secret Message: ");
    while(i--){
        scanf("%s",message);
        strcat(secret, " ");
        strcat(secret, message);
    }
    return strlen(secret);
}

void Help(){
    printf( "Operations:\n" );
    printf( "  i       Inscribe\n" );
    printf( "  e       Extract\n\n" );
}

void changeFilename(char* OutputFileName){
    int l = strlen(OutputFileName);
    while(l+1){
        OutputFileName[l+2] = OutputFileName[l];
        l--;
    }
    OutputFileName[0] = 's';
    OutputFileName[1] = '_';
}

int main( int argc, char **argv ){
    FILE *f;
    unsigned char *InputFileByteStream, *OutputFileByteStream, Operation;
    unsigned int  FileSize, i = 0, count;
    char InputFileName[64], OutputFileName[64], ch, secret[1024];
    printf( "\t\t\tSafeBox\n" );
    Help();
    printf("Enter the Operation: ");
    scanf("%c", &Operation);

    if( (Operation != 'i') && (Operation != 'e') )
    {
        Help();
        return 0;
    }

    printf("\nEnter the Media Input name: ");
    scanf("%s",InputFileName);
    f = fopen( InputFileName, "rb" );
    if( !f )
    {
        printf( "Unable to open input file \"%s\".\n", InputFileName );
        return 0;
    }

    FileSize = GetFileSize( f );
    InputFileByteStream = (unsigned char *) malloc( FileSize );

    if( !InputFileByteStream )
    {
        printf( "Not enough memory\n" );
        fclose( f );
        return 0;
    }

    fread( InputFileByteStream, FileSize, 1, f );
    fclose( f );


    switch( Operation )
    {
        case 'i':
            printf( "\nInscribing the Secret... \n" );
            count = readSecret(secret);
            strcpy(OutputFileName, InputFileName);
            changeFilename(OutputFileName);
            OutputFileByteStream = inscribe(InputFileByteStream, secret, FileSize);
            f = fopen( OutputFileName, "wb" );
            if( !f )
            {
                printf( "Unable to open input file \"%s\".\n", InputFileName );
                return 0;
            }
            fwrite( OutputFileByteStream, FileSize, 1, f );
            fclose(f);
        break;

        case 'e':
            printf( "\nExtracting the Secret... \n" );
            strcpy(secret, extract(InputFileByteStream, FileSize));
            printf("\nSecret Message is '%s'\n",secret);
        break;
    }

    free( InputFileByteStream );
    return 0;
}
