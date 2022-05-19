# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <math.h>
# include <time.h>

typedef struct idNode
{
    unsigned int id;
    struct idNode* maxterm;
    struct idNode* sebelum;
    struct idNode* setelah;
}idNodeT;

/* */
void BuatListMaxterm(bool** perluDicover, idNodeT** listMaxterm, int hitungMintermsTercover, int hitungPrimeImplicantsTercover);
void InisialisasiNode(idNodeT** root, unsigned int id);
void InsertNode(idNodeT* root, unsigned int id);
void SalinNode(idNodeT* root, idNodeT** listMaxterm);
void TambahNode(idNodeT* root, idNodeT* tambahNode, idNodeT* nodeSebelumnya);
void HapusNode(idNodeT* root, idNodeT* sebelumnya);

/* */
void MengalikanOut(idNodeT** listMaxterm, unsigned int panjangKali);
void SimplifikasiSatu(idNodeT** listMaxterm);
void SimplifikasiDua(idNodeT** listMaxterm);
void CoverTerkecil(idNodeT** listMaxterm, idNodeT** maxtermTerpendek, unsigned int* hitungMaxtermTerpendek);


