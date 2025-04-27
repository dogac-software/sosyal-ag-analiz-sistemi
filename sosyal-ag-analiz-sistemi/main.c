#include <stdio.h>
#include <stdlib.h>
#include <locale.h> 

#define MAKS_KULLANICI 100

typedef struct Kullanici {
    int id;
    int arkadaslar[MAKS_KULLANICI];
    int arkadas_sayisi;
} Kullanici;

Kullanici kullanicilar[MAKS_KULLANICI];
int kullanici_sayisi = 0;

// Kullan�c�y� bul veya olu�tur
Kullanici* kullanici_getir_veya_olustur(int id) {
    int i;
    for (i = 0; i < kullanici_sayisi; i++) {
        if (kullanicilar[i].id == id)
            return &kullanicilar[i];
    }
    if (kullanici_sayisi >= MAKS_KULLANICI) {
        printf("Maksimum kullan�c� say�s�na ula��ld�.\n");
        return NULL;
    }
    kullanicilar[kullanici_sayisi].id = id;
    kullanicilar[kullanici_sayisi].arkadas_sayisi = 0;
    kullanici_sayisi++;
    return &kullanicilar[kullanici_sayisi - 1];
}

// Arkada�l�k ekle
void arkadaslik_ekle(int id1, int id2) {
    Kullanici* k1 = kullanici_getir_veya_olustur(id1);
    Kullanici* k2 = kullanici_getir_veya_olustur(id2);
    if (k1 && k2) {
        k1->arkadaslar[k1->arkadas_sayisi++] = id2;
        k2->arkadaslar[k2->arkadas_sayisi++] = id1;
    }
}

// Belirli derinlikteki arkada�lar� DFS ile bul
void derinlikte_ara(Kullanici* kullanici, int derinlik, int ziyaret_edilen[], int mevcut_derinlik, int kaynak_id) {
    int i;
    if (mevcut_derinlik > derinlik)
        return;
    ziyaret_edilen[kullanici->id] = 1;
    if (mevcut_derinlik == derinlik && kullanici->id != kaynak_id)
        printf("Kullan�c� %d mesafe %d uzakl�kta\n", kullanici->id, derinlik);
    for (i = 0; i < kullanici->arkadas_sayisi; i++) {
        int ark_id = kullanici->arkadaslar[i];
        if (!ziyaret_edilen[ark_id]) {
            Kullanici* ark = kullanici_getir_veya_olustur(ark_id);
            if (ark) {
                derinlikte_ara(ark, derinlik, ziyaret_edilen, mevcut_derinlik + 1, kaynak_id);
            }
        }
    }
}

// Ortak arkada�lar� bul
void ortak_arkadaslar(int id1, int id2) {
    int i, j;
    Kullanici* k1 = kullanici_getir_veya_olustur(id1);
    Kullanici* k2 = kullanici_getir_veya_olustur(id2);
    printf("%d ve %d i�in ortak arkada�lar: ", id1, id2);
    if (k1 && k2) {
        for (i = 0; i < k1->arkadas_sayisi; i++) {
            for (j = 0; j < k2->arkadas_sayisi; j++) {
                if (k1->arkadaslar[i] == k2->arkadaslar[j])
                    printf("%d ", k1->arkadaslar[i]);
            }
        }
    }
    printf("\n");
}

// Etki puan� hesapla
int etki_puani(int id) {
    Kullanici* kullanici = kullanici_getir_veya_olustur(id);
    int puan = 0;
    int gorulen[MAKS_KULLANICI] = {0};
    int i, j;

    if (!kullanici) return 0;

    for (i = 0; i < kullanici->arkadas_sayisi; i++) {
        int a_id = kullanici->arkadaslar[i];
        gorulen[a_id] = 1;
        puan++;
        Kullanici* arkadas = kullanici_getir_veya_olustur(a_id);
        if (arkadas) {
            for (j = 0; j < arkadas->arkadas_sayisi; j++) {
                int a_a_id = arkadas->arkadaslar[j];
                if (!gorulen[a_a_id] && a_a_id != id) {
                    gorulen[a_a_id] = 1;
                    puan++;
                }
            }
        }
    }
    return puan;
}

// Topluluk gez (ba�l� bile�enler)
void topluluk_gez(Kullanici* kullanici, int ziyaret_edilen[]) {
    int i;
    ziyaret_edilen[kullanici->id] = 1; // Kullan�c�y� ziyaret etti�imizi i�aretle
    printf("%d ", kullanici->id); // Kullan�c�y� yazd�r

    // Arkada�lar�n� gez
    for (i = 0; i < kullanici->arkadas_sayisi; i++) {
        int a_id = kullanici->arkadaslar[i];
        if (!ziyaret_edilen[a_id]) { // E�er arkada� ziyaret edilmemi�se
            Kullanici* ark = kullanici_getir_veya_olustur(a_id);
            if (ark) {
                topluluk_gez(ark, ziyaret_edilen); // Arkada�� gez
            }
        }
    }
}

// BST d���m yap�s�
typedef struct AgacDugum {
    int id;
    struct AgacDugum *sol, *sag;
} AgacDugum;

// BST'ye eleman ekle
AgacDugum* agaca_ekle(AgacDugum* kok, int id) {
    if (kok == NULL) {
        AgacDugum* dugum = (AgacDugum*)malloc(sizeof(AgacDugum));
        dugum->id = id;
        dugum->sol = dugum->sag = NULL;
        return dugum;
    }
    if (id < kok->id)
        kok->sol = agaca_ekle(kok->sol, id);
    else if (id > kok->id)
        kok->sag = agaca_ekle(kok->sag, id);
    return kok;
}

// BST'de arama
AgacDugum* agacta_ara(AgacDugum* kok, int id) {
    if (kok == NULL || kok->id == id)
        return kok;
    if (id < kok->id)
        return agacta_ara(kok->sol, id);
    else
        return agacta_ara(kok->sag, id);
}

int main() {
    int i;

    setlocale(LC_ALL, "Turkish");

    // �rnek arkada�l�klar
    arkadaslik_ekle(101, 102);
    arkadaslik_ekle(101, 103);
    arkadaslik_ekle(102, 104);
    arkadaslik_ekle(103, 104);
    arkadaslik_ekle(104, 105);

    // 2. Derinlikteki arkada�lar (101)
    printf("== Derinlik 2'deki arkada�lar (101): ==\n");
    int ziyaret_edilen[MAKS_KULLANICI] = {0};
    derinlikte_ara(kullanici_getir_veya_olustur(101), 2, ziyaret_edilen, 0, 101);

    // Ortak arkada�lar
    printf("\n== Ortak arkada�lar (102 ve 103): ==\n");
    ortak_arkadaslar(102, 103);

    // Etki puan�
    printf("\n== Etki puan� (104): ==\n");
    int puan = etki_puani(104);
    printf("Etki puan�: %d\n", puan);

    // Topluluklar
    printf("\n== Topluluklar: ==\n");
    int topluluk_z[MAKS_KULLANICI] = {0}; // Ziyaret edilenleri tutacak dizi
    for (i = 0; i < kullanici_sayisi; i++) {
        if (!topluluk_z[kullanicilar[i].id]) { // E�er bu kullan�c� ziyaret edilmemi�se
            printf("Topluluk: ");
            topluluk_gez(&kullanicilar[i], topluluk_z); // Bu kullan�c�dan ba�layarak toplulu�u gez
            printf("\n");
        }
    }

    // Kullan�c� Arama (�kili A�a�)
    printf("\n== Kullan�c� Arama (�kili A�a�): ==\n");
    AgacDugum* kok = NULL;
    for (i = 0; i < kullanici_sayisi; i++) {
        kok = agaca_ekle(kok, kullanicilar[i].id);
    }

    int aranan = 104;
    AgacDugum* sonuc = agacta_ara(kok, aranan);
    if (sonuc)
        printf("Kullan�c� %d bulundu.\n", sonuc->id);
    else
        printf("Kullan�c� bulunamad�.\n");

    return 0;
}

