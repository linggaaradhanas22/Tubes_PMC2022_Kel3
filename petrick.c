# include "petrick_lib.h"

void BuatListMaxterm(bool** perluDicover, idNodeT** listMaxterm, int hitungMintermsTercover, int hitungPrimeImplicantsTercover)
{
    unsigned int i, j;
    bool penanda = false;

    for(j = 0; j <hitungMintermsTercover; j++, penanda = false)
    {
        for(i = 0; i < hitungPrimeImplicantsTercover; i++)
        {
            if (perluDicover[i][j] == true)
            {
                if(penanda == false)
                {
                    InisialisasiNode(&listMaxterm[j], i);
                    penanda = true;
                }
                else 
                    InsertNode(listMaxterm[j], i);
            }
        }
    }
}

void CoverTerkecil(idNodeT** listMaxterm, idNodeT** maxtermTerpendek, unsigned int* hitungMaxtermTerpendek){
    unsigned int i,min = 4294967295;
    idNodeT *curr0 , *curr1;
        for (curr0 = listMaxterm[0], i = 0; curr0 != NULL; curr0 = curr0->setelah, i = 0) {
            for (curr1 = curr0; curr1 != NULL; curr1 = curr1->maxterm) {
                i++;
            }
            if(i < min){
                min = i; 
                *maxtermTerpendek = curr0;
            }
        }
    *hitungMaxtermTerpendek = min;
    
    if ((*maxtermTerpendek)->sebelum != NULL)
        (*maxtermTerpendek)->sebelum->setelah = (*maxtermTerpendek)->setelah;
    else if((*maxtermTerpendek)->sebelum == NULL && (*maxtermTerpendek)->setelah != NULL)
        listMaxterm[0] = (*maxtermTerpendek)->setelah;
    else
        listMaxterm[0] = NULL;
    if ((*maxtermTerpendek)->setelah != NULL)
        (*maxtermTerpendek)->setelah->sebelum = (*maxtermTerpendek)->sebelum;
}

void InisialisasiNode(idNodeT** root, unsigned int id)
{
    idNodeT* nodeBaru = (idNodeT*) malloc(sizeof(idNodeT));
    nodeBaru->id = id;
    nodeBaru->setelah = nodeBaru->sebelum = nodeBaru->maxterm = NULL;
    *root = nodeBaru;
}

void InsertNode(idNodeT* root, unsigned int id)
{
    idNodeT* nodeBaru = (idNodeT*) malloc(sizeof(idNodeT));
    nodeBaru->id = id;
    nodeBaru->setelah = nodeBaru->sebelum = nodeBaru->maxterm = NULL;

    idNodeT* saatIni;
    for(saatIni = root; saatIni->setelah != NULL; saatIni = saatIni->setelah)
        if(saatIni->setelah->id > nodeBaru->id)
            break;

    if(saatIni->setelah != NULL)
    {
        saatIni->setelah->sebelum = nodeBaru;
        nodeBaru->setelah = saatIni->setelah;
        nodeBaru->sebelum = saatIni;
        saatIni->setelah = nodeBaru;
    }
    else
    {
        nodeBaru->sebelum = saatIni;
        saatIni->setelah = nodeBaru;
    }
}

void SalinNode(idNodeT* root, idNodeT** listMaxterm)
{
    idNodeT* nodeBaru = (idNodeT*) malloc(sizeof(idNodeT));
    nodeBaru->id = root->id;
    nodeBaru->setelah = nodeBaru->sebelum = nodeBaru->maxterm = NULL;

    // Salin semua maxterm
    idNodeT* saatIni0;
    idNodeT* saatIni1;
    for(saatIni0 = root, saatIni1 = nodeBaru; saatIni0->maxterm != NULL; saatIni0 = saatIni0->maxterm)
    {
        idNodeT* idNode = (idNodeT*) malloc(sizeof(idNodeT));
        idNode->id = saatIni0->maxterm->id;
        saatIni1->maxterm = idNode;
        saatIni1 = saatIni1->maxterm;
    }

    //Menyesuaikan indikator untuk node sebelum dan sesudah
    if(root->setelah != NULL)
    {
        root->setelah->sebelum = nodeBaru;
        nodeBaru->setelah = root->setelah;
        nodeBaru->sebelum = root;
        root->setelah = nodeBaru;
    }
    else
    {
        nodeBaru->setelah = root;
        root->setelah = nodeBaru;
    }
}

void TambahNode(idNodeT* root, idNodeT* tambahNode, idNodeT* nodeSebelumnya)
{
    idNodeT* idNode = (idNodeT*) malloc(sizeof(idNodeT));
    idNode->id = tambahNode->id;
    idNode->setelah = idNode->sebelum = idNode->maxterm = NULL;

    if(root->id <= idNode->id)
    {
        idNodeT* saatIni;
        for(saatIni = root; saatIni->maxterm != NULL; saatIni = saatIni->maxterm)
            if(saatIni->maxterm->id > idNode->id)
                break;
        
        if(saatIni->maxterm != NULL)
        {
            idNode->maxterm = saatIni->maxterm;
            saatIni->maxterm = idNode;
        }
        else 
            saatIni->maxterm = idNode;
    }
    else
    {
        idNode->setelah = root->setelah;
        idNode->sebelum = root->sebelum;
        idNode->maxterm = root;
        if(root->setelah != NULL)
            root->setelah->sebelum = idNode;
        nodeSebelumnya->setelah = idNode;
    }
}

void HapusNode(idNodeT* root, idNodeT* sebelumnya)
{
    sebelumnya->maxterm = root->maxterm;
    free(root);
    root = NULL;
}

void MengalikanOut(idNodeT** listMaxterm, unsigned int panjangKali)
{
    idNodeT* saatIni0;
    idNodeT* saatIni1;
    idNodeT* saatIniSebelum;

    // Menentukan jumlah yang harus dikalikan
    unsigned int hitungMaxtermsSelanjutnya = 0, hitungPerkalian;
    for(saatIni0 = listMaxterm[0]; saatIni0 != NULL; saatIni0 = saatIni1)
    {
        for(hitungPerkalian = 0, saatIni1 = saatIni0->setelah; hitungPerkalian < hitungMaxtermsSelanjutnya; hitungPerkalian++)
        {
            SalinNode(saatIni0, listMaxterm);
        }
    }

    // Menghubungkan tiap pengali
    for(saatIni0 = listMaxterm[0], saatIniSebelum = NULL, saatIni1 = listMaxterm[panjangKali]; saatIni0 != NULL; saatIni0 = saatIni0->setelah, saatIni1 = saatIni1->setelah)
    {
        if(saatIni1 != NULL)
        {
            TambahNode(saatIni0, saatIni1, saatIniSebelum);
            saatIniSebelum = saatIni0;
        }
        else
        {
            saatIni1 = listMaxterm[panjangKali];
            TambahNode(saatIni0, saatIni1, saatIniSebelum);
            saatIniSebelum = saatIni0;
        }
    }
}

void SimplifikasiSatu(idNodeT** listMaxterm)
{
    idNodeT* saatIni0;
    idNodeT* saatIni1;
    idNodeT* saatIni2;
    idNodeT* saatIniTemp;

    for(saatIni0 = listMaxterm[0]; saatIni0 != NULL; saatIni0 = saatIni0->setelah)
    {
        for(saatIni1 = saatIni0; saatIni1 != NULL; saatIni1 = saatIni1->maxterm)
        {
            for(saatIni2 = saatIni1->maxterm, saatIniTemp = saatIni1; saatIni2 != NULL;)
            {
                if(saatIni2->id == saatIni1->id)
                {
                    HapusNode(saatIni2,saatIniTemp);
                    saatIni2 = saatIniTemp->maxterm;
                }
                else
                {
                    saatIniTemp = saatIni2;
                    saatIni2 = saatIni2->maxterm;
                }
            }
        }
    }    
}

void SimplifikasiDua(idNodeT** listMaxterm)
{
    idNodeT* saatIni0;
    idNodeT* saatIni1;
    idNodeT* saatIniTemp0;
    idNodeT* saatIniTemp1;
    bool penanda;

    for(saatIni0 = listMaxterm[0]; saatIni0 != NULL; saatIni0 = saatIni0->setelah)
    {
        for(saatIni1 = saatIni0->setelah, penanda = true; saatIni1 != NULL; saatIni1 = saatIni1->setelah, penanda = true)
        {
            for(saatIniTemp0 = saatIni0, saatIniTemp1 = saatIni1; saatIniTemp0 != NULL && saatIniTemp1 != NULL; saatIniTemp0 = saatIniTemp0->maxterm, saatIniTemp1 = saatIniTemp1->maxterm)
            {
                if(saatIniTemp0->id != saatIniTemp1->id)
                {
                    penanda = false;
                    break;
                }
            }
            if(penanda == true && saatIniTemp0 == NULL)
            {
                saatIni1->sebelum->setelah = saatIni1->setelah;
                if(saatIni1->setelah != NULL)
                    saatIni1->setelah->sebelum = saatIni1->sebelum;
            }

            else if(penanda == true && saatIniTemp1 == NULL && saatIni0->sebelum != NULL)
            {
                saatIni0->sebelum->setelah = saatIni0->setelah;
                if(saatIni0->setelah != NULL)
                    saatIni0->setelah->sebelum = saatIni0->sebelum;
                saatIni0 = saatIni0->setelah;
            }

            else if(penanda == true && saatIni1 == NULL && saatIni0->sebelum == NULL)
            {
                listMaxterm[0] = saatIni0->setelah;
                if(saatIni0 != NULL)
                    saatIni0->setelah->sebelum = NULL;
                saatIni0 = saatIni0->setelah;
            } 
        }
    }
}
