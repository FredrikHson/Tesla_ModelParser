#include <stdio.h>

const char* NodeType_Names[] =
{
    "Root",
    "Transform",
    "Object",
    "Mesh",
    "VertexArray",
    "IndexArray",
    "Material"
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

char* readLenChar(FILE* f)
{
    unsigned int len = 0;
    fread(&len, 4, 1, f);
    char* text = new char[len + 1]; // include zero termination

    fread(text, len, 1, f);
    text[len] = 0;

    return text;
}

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

void readIndexArrayData(FILE* f)
{

    unsigned char sides = 0;
    unsigned int arraylen = 0;
    fread(&sides, 1, 1, f);

    fread(&arraylen, 4, 1, f);
    unsigned int* indexdata = new unsigned int[arraylen];

    fread(indexdata, arraylen * 4, 1, f);
    fprintf(stdout, "\tsides:%i\n\tarraylen:%i\n", sides, arraylen);
    int j = 0;

    for(int i = 0; i < arraylen; i++)
    {
        if(j == 0)
        {
            fprintf(stdout, "\t");
        }

        fprintf(stdout, "%i", indexdata[i]);
        j++;

        if(j >= sides)
        {
            fprintf(stdout, "\n");
            j = 0;
        }
        else
        {
            fprintf(stdout, ", ");
        }
    }

    if(indexdata)
    {
        delete [] indexdata;
    }
}
void readVertexArrayData(FILE* f)
{
    char* attrib = readLenChar(f);

    unsigned char vertlength = 0;
    unsigned int arraylen = 0;
    fread(&vertlength, 1, 1, f);

    fread(&arraylen, 4, 1, f);
    float* vertexdata = new float[arraylen];

    fread(vertexdata, arraylen * 4, 1, f);
    fprintf(stdout, "\tattrib:%s\n\tvertlength:%i\n\tarraylen:%i\n", attrib, vertlength, arraylen);
    int j = 0;

    for(int i = 0; i < arraylen; i++)
    {
        if(j == 0)
        {
            fprintf(stdout, "\t");
        }

        fprintf(stdout, "%f", vertexdata[i]);
        j++;

        if(j >= vertlength)
        {
            fprintf(stdout, "\n");
            j = 0;
        }
        else
        {
            fprintf(stdout, ", ");
        }
    }

    if(attrib)
    {
        delete [] attrib;
    }

    if(vertexdata)
    {
        delete [] vertexdata;
    }
}

void readMaterialData(FILE* f)
{
    char* name = readLenChar(f);
    fprintf(stdout, "\tname:%s\n", name);

    if(name)
    {
        delete [] name;
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

    fprintf(stdout, "Node\n\tSize:%i Type:%s numchildren:%i\n", size, NodeType_Names[type], numchildren);

    switch(type)
    {
        case Transform:
        {
            readTranformData(f);
            break;
        }

        case VertexArray:
        {
            readVertexArrayData(f);
            break;
        }

        case IndexArray:
        {
            readIndexArrayData(f);
            break;
        }
        case Material:
        {
            readMaterialData(f);
            break;
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
