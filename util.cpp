#define InvalidCodePath(Message) assert(!"Invalid Code Path" && (Message))
#define NotDone(Message) assert(!"Not Done" && (Message))

const u64 UnsignedLongMax = 9223372036854775807;
void FatalError(const char* ErrorMsg)
{
    printf("%s\n", ErrorMsg);
    exit(1);
}

// TODO: error handling
char* ReadFile(const char* Filename)
{
    char* Result = 0;
    FILE* File = fopen(Filename, "rb");
    if(!File)
    {
        FatalError("Could not open file");
        exit(2);
    }
    assert(File);

    fseek(File, 0, SEEK_END);
    u32 FileLength = ftell(File);
    fseek(File, 0, SEEK_SET);

    Result = (char* )malloc(FileLength + 1);
    assert(Result);
    fread(Result, FileLength, 1, File);
    Result[FileLength] = 0;

    fclose(File);
    return Result;
}

template<class T>
struct array
{
    u32 Size = 0;
    u32 Capacity = 0;
    T* Data = 0;
};

template<class T>
void AddToArray(array<T>* Array, T Element)
{
    if(Array->Data)
    {
        if(Array->Size == Array->Capacity)
        {
            Array->Capacity *= 2;
            Array->Data = (T* )realloc(Array->Data, Array->Capacity * sizeof(T));
        }
    }
    else
    {
        Array->Size = 0;
        Array->Capacity = 10;
        Array->Data = (T* )malloc(Array->Capacity * sizeof(T));
    }

    Array->Data[Array->Size] = Element;
    Array->Size++;
}
