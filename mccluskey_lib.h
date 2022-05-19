# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <math.h>
# include <time.h>

# define DELIMITER ','
# define PARSE_DELIMETERS " ,"
# define PANJANG_MAKS_EKSPRESI 1001
# define MAKS_VARIABEL 32


/* Struktur data yang digunakan */
typedef struct NODE
{
    unsigned int id;
    struct NODE* lanjut;
    struct NODE* sebelum;
}Node; 

typedef struct grupMinterm
{
    char* repr;
    unsigned int hitungPosisiBits;
    Node* root;

}grupMintermT;

/** BacaInput. Prosedur untuk membaca input dari pengguna. Ada dua input yang
 *  dimasukkan oleh pengguna. Yang pertama adalah ekspresi care, dan yang kedua 
 *  adalah ekspresi dont'care. Term yang dimasukan harus menggunakan koma sebagai pemisahnya. 
 * 
 * @param ekspresi_Care input ekspresi care dari pengguna
 * @param eskrpesi_DontCare input ekspresi don't care dari pengguna 
 */
void BacaInput(char* ekspresi_Care, char* ekspresi_DontCare);

/** HitungTerms. Prosedur parsing yang digunakan untuk mendapatkan jumlah terms (baik minterm maupun dont care) 
 *  dari input pengguna. Cara yang digunakan yaitu dengan menghitung banyaknya koma pada input.
 * 
 * @param Term input term dari pengguna yang sudah diolah oleh Prosedur BacaInput
 * @param hitung_Term variabel pointer ke variabel hitung_Term 
 */

void HitungTerms(char* Term, unsigned int* hitung_Term);

/** ParseInput. Prosedur yang digunakan untuk mengolah input sehingga bisa diproses untuk melakukan 
 *  perhitungan minimisasi dengan metode tabular.  
 * 
 * @param ekspresi_minterm input ekspresi yang sudah didapat
 * @param arrayMinterm array yang berisi seluruh minterm
 * @param hitungMinterm banyaknya minterm
 * @param hitungVariabel banyaknya variabel yang diperlukan berdasarkan banyaknya minterm
 */
void ParseInput(char* ekspresi_minterm, grupMintermT* arrayMinterm, unsigned int hitungMinterms, unsigned int* hitungVariabel);

/** KomparasiID. Fungsi yang digunakan untuk membandingkan  
 */
int KomparasiID(const void* a, const void* b);

int KomparsiRerpesentasiBit(const void* a, const void* b);

bool BisaMembentukGrup(grupMintermT kelompokPertama, grupMintermT kelompokKedua, unsigned int hitungVariabel);

void BuatRepresentasiBaru(char* RepresentasiGrupBaru, char* RepresentasiPertama, char* RepresentasiKedua, int hitungVariabel);


/* */

void InisiasiList(Node** root, unsigned int id);

void InsertList(Node** root, unsigned int id);

void GabungList(Node** rootBaru, Node* rootPertama, Node* rootKedua);

bool ListSama(Node* rootPertama, Node* rootKedua);

void CetakList(Node* root);

/* */
void AmbilPrimeImplicant(grupMintermT** tabel, bool** termDigunakan, grupMintermT* arrayPrimeImplicants, unsigned int* panjangKol, int hitungKolom);

void BuatPrimeChart(bool** primeChart, grupMintermT* minterms, int hitungMinterms, grupMintermT* arrayPrimeImplicants, int hitungPrimeImplicants);

void AmbilImplicantEsensial(bool** primeChart, int hitungPrimeImplicants, int hitungMinterms, bool* apakahEsensial);