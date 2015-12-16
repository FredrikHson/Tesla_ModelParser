#include <stdio.h>
const char* NodeTypes[] =
{
    "Root = 0",
    "Transform = 1",
    "Object = 2",
    "Mesh = 3",
    "VertexArray = 4",
    "IndexArray = 5",
    "Material = 6"
};
enum NodeType
{
    Root = 0,
    Transform = 1,
    Object = 2,
    Mesh = 3,
    VertexArray = 4,
    IndexArray = 5,
    Material = 6
};

void readTranformData(FILE* f)
{
    float matrix[16] = {0};
    fread(&matrix, 16 * 4, 1, f);

    for(int i = 0; i < 4; i++)
    {
        int offset = i * 4;
        fprintf(stdout, "\tm[%i]:%f", offset, matrix[offset]);
        fprintf(stdout, "\tm[%i]:%f", offset + 1, matrix[offset + 1]);
        fprintf(stdout, "\tm[%i]:%f", offset + 2, matrix[offset + 2]);
        fprintf(stdout, "\tm[%i]:%f\n", offset + 3, matrix[offset + 3]);
    }
}

bool readNode(FILE* f)
{
    if(feof(f))
    {
        return false;
    }

    unsigned int size          = 0;
    unsigned short numchildren = 0;
    unsigned short type        = 0;

    if(fread(&size, 4, 1, f) == 0)
    {
        return false; // endof file reached
    }

    fread(&type, 2, 1, f);
    fread(&numchildren, 2, 1, f);

    fprintf(stdout, "Node\n\tSize:%i Type:%s numchildren:%i\n", size, NodeTypes[type], numchildren);

    switch(type)
    {
        case Transform:
        {
            readTranformData(f);
        }

        default:
            fseek(f, size, SEEK_CUR);
            break;
    }

    return true;
}


int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        fprintf(stderr, "no file specified\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "rb");

    if(!f)
    {
        fprintf(stderr, "file:%s not found\n", argv[1]);
        return 1;
    }

    char header[6] = {0};

    fread(header, 6, 1, f);
    fprintf(stdout, "Header:%.6s\n", header);
    int i = 0;

    while(readNode(f))
    {
    }


    fclose(f);
    return 0;
}
