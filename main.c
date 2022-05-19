/** EL2008 Pemecahan Masalah dengan C 
 *  TUGAS BESAR
 *  Kelompok            : 3
 *  Anggota             : 1. Mario Jeiba (13220014) 
 *                        2. Michael Andreas Manullang (13220018)
 *                        3. Lingga Aradhana Sahadewa (13220020)                      
 *  Nama File           : main.c 
 *  Deskripsi           : Program untuk melakukan minimisasi fungsi logika dengan menggunakan metode tabular
 */

# include "mccluskey_lib.h"
# include "petrick_lib.h"

int main()
{
    
    /*** Perhitungan minimisasi ***/
    
    /* Deklarasi array untuk menyimpan input. Input dimasukkan dalam format string */
    char ekspresi_Care[PANJANG_MAKS_EKSPRESI];
    char ekspresi_DontCare[PANJANG_MAKS_EKSPRESI];

    /* Menerima Input */
    BacaInput(ekspresi_Care, ekspresi_DontCare);
    
    /* Memulai timer pada program */
    clock_t mulai, selesai;
    mulai = clock();

    /* Menghitung banyaknya minterm Care dan Don't care dari input */
    unsigned int hitungMintermsCare, hitungDontCare;
    HitungTerms(ekspresi_Care, &hitungMintermsCare);
    HitungTerms(ekspresi_DontCare, &hitungDontCare);

    /* Penggabungan ekspresi Care dan Dont Care saat pengguna menginput selain -1  
    saat prompt "Masukan dont care" muncul. Bila terdapat ekspresi dont care, program
    ekspresi care dan dont care akan digabung. */
    char* ekspresi_minterms = (char*) malloc(PANJANG_MAKS_EKSPRESI * sizeof(char));      
    memcpy(ekspresi_minterms, ekspresi_Care, PANJANG_MAKS_EKSPRESI * sizeof(char));
    unsigned int hitungMinterms = hitungMintermsCare;

    // Memeriksa apakah ekspresi dont care tidak mengandung input -1
    if (strstr(ekspresi_DontCare, "-1") == NULL)
    {
        ekspresi_minterms = strcat(strcat(ekspresi_minterms, ","), ekspresi_DontCare);
        hitungMinterms += hitungDontCare;
    }

    /* Mengubah input minterm ke dalam bentuk array bertipe minterm dan
    melakukan analisis terhadap input tersebut */
    grupMintermT* arrayMinterm = (grupMintermT*) malloc(hitungMinterms * sizeof(grupMintermT));
    unsigned int hitungVariabel;
    ParseInput(ekspresi_minterms, arrayMinterm, hitungMinterms, &hitungVariabel);

    /* Mengurutkan array minterm berdasarkan id mintermnya menggunakan algoritma
    quicksort */
    qsort(arrayMinterm, hitungMinterms, sizeof(grupMintermT), KomparasiID);

    /* Menentukan panjang tabel perhitungan berdasarkan banyaknya implikan */
    unsigned int panjangKol[MAKS_VARIABEL];
    panjangKol[0] = hitungMinterms;

    /* Menyimpan arrayMinterm ke dalam tabel 0 dan mengurutkannya berdasarkan angka satu */
    grupMintermT* tabel[MAKS_VARIABEL + 1];
    tabel[0] = (grupMintermT*) malloc(hitungMinterms * sizeof(grupMintermT));
    memcpy(tabel[0], arrayMinterm, panjangKol[0] * sizeof(grupMintermT));
    qsort(tabel[0], hitungMinterms, sizeof(grupMintermT), KomparsiRerpesentasiBit);

    /* Mencatat penggabungan implikan saat terjadi */
    bool* termDigunakan[MAKS_VARIABEL + 1];
    termDigunakan[0] = (bool*) calloc(panjangKol[0], sizeof(bool));

    /* Mencatat banyaknya prime implicants */
    unsigned int hitungPrimeImplicants = 0;

    /* Membuat tabel untuk menemukan semua implikan */
    unsigned int i,j,k; // variabel untuk pengiterasian
    for(i = 0; i <= hitungVariabel; i++)
    {
        grupMintermT* kolomSelanjutnya = (grupMintermT*) malloc((panjangKol[i] * panjangKol[i]) * sizeof(grupMintermT));
        int posisiKolomSelanjutnya = 0;

        for(j = 0, k = 1; j < panjangKol[i]; j++, k = j + 1)
        {
            while(k < panjangKol[i] && tabel[i][k].hitungPosisiBits == tabel[i][j].hitungPosisiBits)
                k++;
            
            while(k < panjangKol[i] && (tabel[i][k].hitungPosisiBits - tabel[i][j].hitungPosisiBits) == 1)
            {
                if(BisaMembentukGrup(tabel[i][k], tabel[i][j], hitungVariabel))
                {
                    // Membuat implikan baru
                    Node* rootNodeBaru;
                    GabungList(&rootNodeBaru, tabel[i][j].root, tabel[i][k].root);

                    // Memeriksa apakah suatu implikan sudah ada pada kolom terbaru
                    bool adaDiKolom = false;
                    for (unsigned int hitung = 0; hitung < posisiKolomSelanjutnya; hitung++)
                    {
                        if(ListSama(kolomSelanjutnya[hitung].root, rootNodeBaru) != 0)
                        {
                            adaDiKolom = true;
                            break;
                        }
                    } 
                    
                    // Bila implikan baru belum ada di kolom yang baru, simpan implikan
                    // tersebut di kolom baru
                    if(adaDiKolom == false)
                    {
                        kolomSelanjutnya[posisiKolomSelanjutnya].root = rootNodeBaru;
                        kolomSelanjutnya[posisiKolomSelanjutnya].hitungPosisiBits = tabel[i][j].hitungPosisiBits;
                        kolomSelanjutnya[posisiKolomSelanjutnya].repr = (char*) malloc((hitungVariabel + 1) * sizeof(char));
                        BuatRepresentasiBaru(kolomSelanjutnya[posisiKolomSelanjutnya].repr, tabel[i][j].repr, tabel[i][k].repr, hitungVariabel);

                        posisiKolomSelanjutnya++;
                    }

                    termDigunakan[i][j] = termDigunakan [i][k] = true;
                }
                k++;
            }
        }

        // Menghitung jumlah implikan yang tidak digunakan (prime implicant)
        for(j = 0; j < panjangKol[i]; j++)
            if(termDigunakan[i][j] == false)
                hitungPrimeImplicants++;
        
        // Mencatat banyaknya implikan untuk kolom baru
        panjangKol[i + 1] = posisiKolomSelanjutnya;

        // Menghentikan pembuatan tabel saat penggabungan implikan tidak memungkinkan lagi
        if(panjangKol[i + 1] == 0)
            break;
        
        // Memindahkan isi kolom baru ke tabel
        tabel[i + 1] = (grupMintermT*) malloc(panjangKol[i + 1] * sizeof(grupMintermT));
        for(j = 0; j < panjangKol[i + 1]; j++)
            tabel[i + 1][j] = kolomSelanjutnya[j];
        
        // Menginisialisasi tabel cek term yang sudah digunakan dan membebaskan alokasi memori
        termDigunakan[i + 1] = (bool*) calloc(panjangKol[i + 1], sizeof(bool));
        free(kolomSelanjutnya);
    }

    /* Menentukan jumlah kolom yang ada di tabel */
    int hitungKolom = i;

    /* Mengorganisasikan prime implicant */
    grupMintermT* arrayPrimeImplicants = (grupMintermT*) malloc(hitungPrimeImplicants * sizeof(grupMintermT));
    AmbilPrimeImplicant(tabel, termDigunakan, arrayPrimeImplicants, panjangKol, hitungKolom);
    
    /* Tabel cover prime implicants */
    bool** primeChart = (bool**) malloc(hitungPrimeImplicants * sizeof(bool*));
    for(i = 0; i < hitungPrimeImplicants; i++)
        primeChart[i] = (bool*) calloc(hitungMintermsCare, sizeof(bool));
    

    /* Membuat array minterm tanpa dont care */
    grupMintermT* arrayMintermCare = (grupMintermT*) malloc(hitungMintermsCare * sizeof(grupMintermT));
    unsigned int hitungVariabelCare;
    ParseInput(ekspresi_Care, arrayMinterm, hitungMintermsCare, &hitungVariabelCare);
    // Mengurutkan array berdasarkan ID minterm 
    qsort(arrayMintermCare, hitungMintermsCare, sizeof(grupMintermT), KomparasiID);

    BuatPrimeChart(primeChart, arrayMintermCare, hitungMintermsCare, arrayPrimeImplicants, hitungPrimeImplicants);

    /* Buat daftar prime implicant esensial */
    bool *apakahEsensial = (bool*) calloc(hitungPrimeImplicants, sizeof(bool));
    AmbilImplicantEsensial(primeChart, hitungPrimeImplicants, hitungMintermsCare, apakahEsensial);

    /* Membuat daftar minterms covered */
    bool* mintermsTercover = (bool*) calloc(hitungMintermsCare, sizeof(bool));
    for(i = 0; i < hitungPrimeImplicants; i++)
        if(apakahEsensial[i] == true)
            for(j = 0; j < hitungMintermsCare; j++)
                if(primeChart[i][j] == true)
                    mintermsTercover[j] = true;

    
    /*** Menampilkan Output ***/

    /* Mencetak tabel penyederhanaan minterm */
    for(i = 0; i <= hitungKolom; i++)
    {
        printf("\n\n###############                  Size %d                   ###############\n\n",(int)pow(2,i));
        for(j = 0; j < panjangKol[i]; j++)
        {
            printf("[ %s ] %s [ ", (termDigunakan[i][j] ? "OK" : " X"), tabel[i][j].repr);
            CetakList(tabel[i][j].root);
            printf("]\n");
        }
    }

    /* Mencetak prime implicant */
    printf("\n\n###############              Prime Implicant              ###############\n\n");
    for(i = 0; i < hitungPrimeImplicants; i++)
    {
        printf("[P%2d]: %s  [ ", i, arrayPrimeImplicants[i].repr);
        CetakList(arrayPrimeImplicants[i].root);
        printf("]\n");
    }

    /* Mencetak prime implicants cover */
    printf("\n\n###############          Prime Implicants Cover           ###############\n\n");
    printf("        ");

    for(j = 0; j < hitungMintermsCare; j++)
        printf("| %2u", arrayMintermCare[j].root->id);
    printf("|\n---------");

    for(j = 0; j < hitungMintermsCare; j++)
        printf("----");
    printf("\n");

    for(i = 0; i < hitungPrimeImplicants; i++)
    {
        printf("[P%2d]:  ", i);
        for(j = 0; j < hitungMintermsCare; j++)
            printf("|  %c", ( primeChart[i][j] == true) ? 'X' : ' ');
        printf("|\n");
    }

    /* Mencetak prime implicants esensial */
    printf("\n\n###############         Essential Prime Implicant         ###############\n\n");
    for(i = 0; i < hitungPrimeImplicants; i++)
    {
        if(apakahEsensial[i] == true)
        {
            printf("[P%2d]: %s  [ ", i, arrayPrimeImplicants[i].repr);
            CetakList(arrayPrimeImplicants[i].root);
            printf(" ]\n");
        }
    }

    /* Mencetak cover prime implicant esensial */
    printf("\n\n###############      Essential Prime Implicant Cover      ###############\n\n");
    for(j = 0; j < hitungMintermsCare; j++)
        printf("| %2u", arrayMintermCare[j].root->id);
    printf("|\n");

	for(j = 0; j < hitungMintermsCare; j++)
		printf("----");
	printf("-\n");

	for(i = 0; i < hitungMintermsCare; i++)
		printf("|  %c", (mintermsTercover[i] == true) ? 'X' : ' ');
	printf("|\n");       

    /* Mencetak Fungsi Prime Implicant Esensial */
    char variabel[32] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f'};
    unsigned int hitungEsensial = 0;
    for (i = 0; i < hitungPrimeImplicants ; i++)
        if (apakahEsensial[i] == true)
            hitungEsensial++;  

    /* Memeriksa apakah seluruh minterms telah tercover */
    bool perhitunganSelesai = true;
    for(i = 0; i < hitungMintermsCare; i++)
        if(mintermsTercover[i] == false)
        {
            perhitunganSelesai = false;
            break;
        }

    if(perhitunganSelesai == true)
    {
        printf("\n\n###############            The Final Function             ###############\n\n");
        printf("F(");
        for(i = 0; i < hitungVariabel; i++)
            printf("%c%s",variabel[i],( i+1 < hitungVariabel) ? "," : ") = ");

        bool apakahSatuVariabel = true;

        for(unsigned int penanda = hitungEsensial, i = 0; i < hitungPrimeImplicants; i++)
        {
            if(apakahEsensial[i] == true)
            {
                for(j = 0; arrayPrimeImplicants[i].repr[j] != '\0'; j++)
                {
                    if(arrayPrimeImplicants[i].repr[j] == '1')
                    {
                        printf("%c",variabel[j]);
                        apakahSatuVariabel = false;
                    }

                    else if(arrayPrimeImplicants[i].repr[j] == '0')
                    {
                        printf("%c\'",variabel[j]);
                        apakahSatuVariabel = false;                
                    }
                }
                if(--penanda)
                    printf(" + ");
            }
        }

        if(apakahSatuVariabel == true)
            printf("1");
    }
    
    printf("\n\n#########################################################################\n");
    printf("###############     Quine-McCluskey Algorithm is Over     ###############\n");
    printf("#########################################################################\n");    

    if(perhitunganSelesai == false)
    {
        printf("\n#########################################################################\n");
        printf("###############        Petrick's Method is Started        ###############\n");
        printf("#########################################################################\n");

        /* Prime implicants untuk membuat overlay prime implicants */
        unsigned int hitungPrimeImplicantsTercover =  hitungPrimeImplicants - hitungEsensial;

        grupMintermT* arrayPrimeImplicantsTercover = (grupMintermT*) malloc(hitungPrimeImplicantsTercover * sizeof(grupMintermT));
        for (i = 0; i < hitungPrimeImplicantsTercover; i++)
        {
            for(j = 0; j< hitungPrimeImplicants; j++)
            {
                if(apakahEsensial[j] == false)
                {
                    memcpy(&arrayPrimeImplicantsTercover[i], &arrayPrimeImplicants[j], sizeof(grupMintermT));
                    j++;
                    break;
                }
            }
        }

        /* Menyiapkan array minterms tercover baru untuk penyederhanaan */
        unsigned int hitungMintermsTercover = 0;
        for(i = 0; i < hitungMintermsCare; i++)
            if(mintermsTercover[i] == false)
                hitungMintermsTercover++;
        grupMintermT* arrayMintermTercover = (grupMintermT*) malloc(hitungMintermsTercover * sizeof(grupMintermT));
        for(i = 0; i < hitungMintermsTercover; i++)
        {
            for(j = 0; j < hitungMintermsCare; j++)
            {
                if(mintermsTercover[j] == false)
                {
                    memcpy(&arrayMintermTercover[i], &arrayMintermCare[j], sizeof(grupMintermT));
                    j++;
                    break;
                }
            }
        }

        /* Membuat tabel dua dimensi untuk menentukan apakah minterm ke j bisa tercover oleh prime implicant ke i */
        bool** perluDicover = (bool**) malloc(hitungPrimeImplicants * sizeof(bool*));
        for(i = 0; i <hitungPrimeImplicants; i++)
            perluDicover[i] = (bool*)calloc(hitungMintermsCare, sizeof(bool));
        BuatPrimeChart(perluDicover, arrayMintermTercover, hitungMintermsTercover, arrayPrimeImplicantsTercover, hitungPrimeImplicantsTercover);
        
        /* Membuat tabel dua dimensi untuk menentukan cover dari setiap minterms */
        printf("\n###############        NEW Prime Implicants Cover         ###############\n\n");
        printf("        ");

        for(j =0; j < hitungMintermsTercover; j++)
            printf("| %2u", arrayMintermTercover[j].root->id);
        printf("|\n---------");

        for(j = 0; j < hitungMintermsTercover; j++)
            printf("----");
        printf("\n");        

        for(i = 0; i < hitungPrimeImplicantsTercover; i++)
        {
            printf("[P%2d]:  ", i);
            for(j = 0; j < hitungMintermsTercover; j++)
                printf("|  %c", (perluDicover[i][j] == true) ? 'X' : ' ');
            printf("|\n");
        }

        idNodeT* listMaxterm[hitungMintermsTercover];
        BuatListMaxterm(perluDicover, listMaxterm, hitungMintermsTercover, hitungPrimeImplicantsTercover);

        printf("\n\n###############               Maxterm List 0              ###############");
        /* Mencetak list maxterm */
        for(j = 0; j < hitungMintermsTercover; j++)
        {
            printf("\n\nColumns %d :\n",j);
            for(idNodeT* saatIni = listMaxterm[j]; saatIni != NULL; saatIni = saatIni->setelah)
                printf("%2u\n",saatIni->id);
        }

        printf("\n");

        /* Mulai mengalikan maxterm dengan aturan penyederhanaan */ 
        for(i = 1; i < hitungMintermsTercover; i++)
        {
            printf("\n\n###############               Maxterm List %d              ###############\n",i);
            MengalikanOut(listMaxterm, i);

            // Cetak list maxterm
            for(j = 0; j <hitungMintermsTercover; j++)
            {
                printf("\nColumns %d :\n",j);
                for(idNodeT* saatIni0 = listMaxterm[j]; saatIni0 != NULL; saatIni0 = saatIni0->setelah)
                {
                    for(idNodeT* saatIni1 = saatIni0; saatIni1 != NULL; saatIni1 = saatIni1->maxterm)
                    {
                        printf("%2u ",saatIni1->id);
                    }
                    printf("\n");
                }
            }

            printf("\n\n###############         Maxterm List %d After XX = X       ###############\n",i);

            SimplifikasiSatu(listMaxterm);

            for(j = 0; j < hitungMintermsTercover; j++)
            {
                printf("\nColumns %d :\n",j);
                for(idNodeT* saatIni0 = listMaxterm[j]; saatIni0 != NULL; saatIni0 = saatIni0->setelah)
                {
                    for(idNodeT* saatIni1 = saatIni0; saatIni1 != NULL; saatIni1 = saatIni1->maxterm)
                    {
                        printf("%2u ",saatIni1->id);
                    }
                    printf("\n");
                }
            }

            printf("\n\n###############       Maxterm List %d After X + XY = X     ###############\n",i);

            SimplifikasiDua(listMaxterm);

            for(j = 0; j < hitungMintermsTercover; j++)
            {
                printf("\nColumns %d :\n",j);
                for(idNodeT* saatIni0 = listMaxterm[j]; saatIni0 != NULL; saatIni0 = saatIni0->setelah)
                {
                    for(idNodeT* saatIni1 = saatIni0; saatIni1 != NULL; saatIni1 = saatIni1->maxterm)
                    {
                        printf("%2u ",saatIni1->id);
                    }
                    printf("\n");
                }
            }     
        }

        idNodeT* maxtermPendek;
        unsigned int hitungMaxtermPendek;
        CoverTerkecil(listMaxterm, &maxtermPendek, &hitungMaxtermPendek);
        if(hitungMintermsTercover > 1)
        {
            idNodeT* maxtermTemp = maxtermPendek;
            idNodeT* saatIni0 = maxtermPendek;
            unsigned int hitungMaxtermTemp;
            for(CoverTerkecil(listMaxterm, &maxtermTemp, &hitungMaxtermPendek), saatIni0 = maxtermPendek; hitungMaxtermTemp == hitungMaxtermPendek; CoverTerkecil(listMaxterm, &maxtermTemp, &hitungMaxtermPendek))
            {
                saatIni0->setelah = maxtermTemp;
                saatIni0 = saatIni0->setelah;
            } 
            saatIni0->setelah = NULL;
        }

        /* Mencetak maxterm overlay minimal */
        printf("\n\n###############            The least Maxterm              ###############\n\n");
        for(idNodeT* saatIni0 = maxtermPendek; saatIni0 != NULL; saatIni0 = saatIni0->setelah)
        {
            for(idNodeT* saatIni1 = saatIni0; saatIni1 != NULL; saatIni1 = saatIni1->maxterm)
                printf("%2u ",saatIni1->id);
            printf("\n");
        }

        /* Mencetak fungsi akhir */
        printf("\n\n###############            The Final Function             ###############\n");
        for(idNodeT* saatIni2 = maxtermPendek; saatIni2 != NULL; saatIni2 = saatIni2->setelah)
        {
            printf("\nF(");
            for(i = 0; i < hitungVariabel; i++)
                printf("%c%s", variabel[i],( i+1 < hitungVariabel) ? "," : ") = ");
            
            // Mencetak prime implicant esensial
            for(unsigned int penanda = hitungEsensial, i = 0; i < hitungPrimeImplicants; i++)
            {
                if(apakahEsensial[i] == true)
                {
                    for (j = 0; arrayPrimeImplicants[i].repr[j] != '\0'; j++)
                    {
                        if(arrayPrimeImplicants[i].repr[j] == '1')
                            printf("%c", variabel[j]);
                        else if (arrayPrimeImplicants[i].repr[j] == '0')
                            printf("%c\'", variabel[j]);
                    }
                    if(--penanda)
                        printf(" + ");
                }
            }

            if(hitungMaxtermPendek != 0 && hitungEsensial != 0)
                printf(" + ");
        
            // Mencetak maxterm minimum
            unsigned int hitungTambah = hitungMaxtermPendek;
            for(idNodeT* saatIni1 = saatIni2; saatIni1 != NULL; saatIni1 = saatIni1->maxterm)
            {
                for(k = 0; arrayPrimeImplicantsTercover[saatIni1->id].repr[k] != '\0'; k++)
                {
                    if(arrayPrimeImplicantsTercover[saatIni1->id].repr[k] == '1')
                        printf("%c", variabel[k]);
                    else if(arrayPrimeImplicantsTercover[saatIni1->id].repr[k] == '0')
                        printf("%c\'", variabel[k]);
                }
                
                if(--hitungTambah)
                    printf(" + ");
            }
            printf("\n");   
        }

        printf("\n#########################################################################\n");
        printf("#################        Petrick's Method is End        #################\n");
        printf("#########################################################################\n\n");  
    }

    /* Mengakhiri Timer dan program */
    selesai = clock();
    printf("The Running Time is %f sec.\n", (double)(selesai - mulai) / CLOCKS_PER_SEC);
    system("PAUSE");

    return 0;
}
