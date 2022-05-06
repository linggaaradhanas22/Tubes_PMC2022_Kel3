# include "mccluskey_lib.h"

void BacaInput(char* ekspresi_Care, char* ekspresi_DontCare)
{
    printf("\n32bit Quine-McCluskey and Petrick's Method in C v5.03");
    printf("\n\n#########################################################################\n");
    printf("################   Quine-McCluskey Algorithm is Started  ################\n");
    printf("#########################################################################\n\n");
    
	printf("Please enter the minterms(Divided by ,):\n\t>> ");
    scanf("%s", ekspresi_Care);
    
    printf("\nPlease enter the don't care(Divided by ,):\n!!! If you don't need don't care, Please enter -1 !!!\n\t>> ");
    scanf("%s", ekspresi_DontCare);    
}

void HitungTerms(char* Term, unsigned int* hitung_Term)
{
	char *ptr = strchr(Term,DELIMITER);
    for (*hitung_Term = 1; ptr != NULL; (*hitung_Term)++)
        ptr = strchr(ptr + 1, DELIMITER);    
}

void ParseInput(char* ekspresi_minterm, grupMintermT* arrayMinterm, unsigned int hitungMinterms, unsigned int* hitungVariabel)
{
    unsigned int i, j, mintermMaks = 0;

    char* pointerChar = strtok(ekspresi_minterm, PARSE_DELIMETERS);
    
    // Menyimpan minterm ke array minterm
    for(i = 0; i < hitungMinterms; i++)
    {
        arrayMinterm[i].repr = (char* ) malloc((MAKS_VARIABEL + 1) * sizeof(char));
        arrayMinterm[i].hitungPosisiBits = 0;
        arrayMinterm[i].root = NULL;

        // Meng-assing ID ke setiap minterm
        unsigned int id = atoi(pointerChar);
        InisiasiList(&arrayMinterm[i].root, id);

        // Memeriksa minterm maksimum
        mintermMaks = (mintermMaks > id)? mintermMaks : id;

        // Mengkonversi input minterm menjadi string bit
        for(j = 0; j < MAKS_VARIABEL; j++, id /= 2)
        {
            if(id % 2 == 1)
            {
                arrayMinterm[i].repr[j] = '1';
                arrayMinterm[i].hitungPosisiBits++;
            }
            else
                arrayMinterm[i].repr[j] = '0';
        }
        arrayMinterm[i].repr[MAKS_VARIABEL] = '\0';

        // Membaca minterm selanjutnya
        if(i + 1 < hitungMinterms)
            pointerChar = strtok(NULL, PARSE_DELIMETERS);
    }

    // Menghitung jumlah variabel maksimum yang diperlukan dari nilai 
    // minterm maksimum
    (*hitungVariabel) = 0;
    for(int biner = 1; mintermMaks > (biner - 1); (*hitungVariabel)++)
    {
        biner *= 2;

    }

    // Menyederhanakan string biner sehingga didapat jumlah yang sesuai dengan banyaknya variabel
    for(i = 0; i < hitungMinterms; i++)
    {
        arrayMinterm[i].repr = (char*) realloc(arrayMinterm[i].repr, (*hitungVariabel + 1) * sizeof(char));
        arrayMinterm[i].repr[*hitungVariabel] = '\0';

        // Membalikan string biner
        for(unsigned long int low = 0, high = *hitungVariabel - 1; low < high; low++, high--)
        {
            char c = arrayMinterm[i].repr[low];
            arrayMinterm[i].repr[low] = arrayMinterm[i].repr[high];
            arrayMinterm[i].repr[high] = c;
        }
    }
}

int KomparasiID(const void* a, const void* b)
{
    grupMintermT* pertama = (grupMintermT*) a;
    grupMintermT* kedua = (grupMintermT*) b;

    return(pertama->root->id - kedua->root->id);
}

int KomparsiRerpesentasiBit(const void* a, const void* b)
{
    grupMintermT* pertama = (grupMintermT*) a;
    grupMintermT *kedua = (grupMintermT*) b;

    if(pertama->hitungPosisiBits == kedua->hitungPosisiBits)
    {
        return strcmp(pertama->repr, kedua->repr);
    }

    return (pertama->hitungPosisiBits - kedua->hitungPosisiBits);
}

bool BisaMembentukGrup(grupMintermT kelompokPertama, grupMintermT kelompokKedua, unsigned int hitungVariabel)
{
    int i, bitBerbeda = 0;
    for(i = 0; i < hitungVariabel; i++)
    {
        // Kasus saat posisi '-' tidak sama
        if((kelompokPertama.repr[i] == '-' && kelompokKedua.repr[i] != '-') || kelompokPertama.repr[i] != '-' && kelompokKedua.repr[i] == '-')
            return 0;
        // Kasus saat nilai binernya berbeda
        if(kelompokPertama.repr[i] != kelompokKedua.repr[i])
            bitBerbeda++;
    }
    // Kondisi akhir pemeriksaan
    return (bitBerbeda == 1) ? 1 : 0;
}

void BuatRepresentasiBaru(char* RepresentasiGrupBaru, char* RepresentasiPertama, char* RepresentasiKedua, int hitungVariabel)
{
    
}