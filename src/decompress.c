#include "global.h"
#include "malloc.h"
#include "data.h"
#include "decompress.h"
#include "pokemon.h"
#include "text.h"

EWRAM_DATA ALIGNED(4) u8 gDecompressionBuffer[0x4000] = {0};

void LZDecompressWram(const u32 *src, void *dest)
{
    LZ77UnCompWram(src, dest);
}

void LZDecompressVram(const u32 *src, void *dest)
{
    LZ77UnCompVram(src, dest);
}

u16 LoadCompressedSpriteSheet(const struct CompressedSpriteSheet *src)
{
    struct SpriteSheet dest;

    LZ77UnCompWram(src->data, gDecompressionBuffer);
    dest.data = gDecompressionBuffer;
    dest.size = src->size;
    dest.tag = src->tag;
    return LoadSpriteSheet(&dest);
}

void LoadCompressedSpriteSheetOverrideBuffer(const struct CompressedSpriteSheet *src, void *buffer)
{
    struct SpriteSheet dest;

    LZ77UnCompWram(src->data, buffer);
    dest.data = buffer;
    dest.size = src->size;
    dest.tag = src->tag;
    LoadSpriteSheet(&dest);
}

void LoadCompressedSpritePalette(const struct CompressedSpritePalette *src)
{
    struct SpritePalette dest;

    LZ77UnCompWram(src->data, gDecompressionBuffer);
    dest.data = (void*) gDecompressionBuffer;
    dest.tag = src->tag;
    LoadSpritePalette(&dest);
}

void LoadCompressedSpritePaletteOverrideBuffer(const struct CompressedSpritePalette *a, void *buffer)
{
    struct SpritePalette dest;

    LZ77UnCompWram(a->data, buffer);
    dest.data = buffer;
    dest.tag = a->tag;
    LoadSpritePalette(&dest);
}

void DecompressPicFromTable(const struct CompressedSpriteSheet *src, void* buffer, s32 species)
{
    if ((src == &gMonFrontPicTable[species] || src == &gFemaleMonFrontPicTable[species]) && !IsMonValid(species))
        LZ77UnCompWram(gMonFrontPicTable[SPECIES_NONE].data, buffer);
    else
        LZ77UnCompWram(src->data, buffer);
}

void HandleLoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, u8 metGame)
{
    bool32 isFrontPic = FALSE; // backPic

    if (src == &gMonFrontPicTable[species] || src == &gFemaleMonFrontPicTable[species])
        isFrontPic = TRUE; // frontPic

    LoadSpecialPokePic(src, dest, species, personality, isFrontPic, metGame);
}

void LoadSpecialPokePic(const struct CompressedSpriteSheet *src, void *dest, s32 species, u32 personality, bool32 isFrontPic, u8 metGame)
{
    if (species == SPECIES_UNOWN)
    {
        u32 unownSpecies = GetUnownSpecies(personality);

        // The other Unowns are separate from Unown A.
        if (isFrontPic)
            LZ77UnCompWram(gMonFrontPicTable[unownSpecies].data, dest);
        else
            LZ77UnCompWram(gMonBackPicTable[unownSpecies].data, dest);
    }
    else if (species == SPECIES_ARBOK && isFrontPic)
    {
        // Arbok from Kanto are different to Arbok from Hoenn.
        LZ77UnCompWram(gMonFrontPicTable[GetArbokVariant(metGame)].data, dest);
    }
    else if (!IsMonValid(species)) // is species unknown? draw the ? icon
        LZ77UnCompWram(gMonFrontPicTable[SPECIES_NONE].data, dest);
    else if (SpeciesHasGenderDifferenceAndIsFemale(species, personality))
    {
        if (isFrontPic)
            LZ77UnCompWram(gFemaleMonFrontPicTable[species].data, dest);
        else
            LZ77UnCompWram(gFemaleMonBackPicTable[species].data, dest);
    }
    else
        LZ77UnCompWram(src->data, dest);
    if (species == SPECIES_SPINDA && isFrontPic)
    {
        DrawSpindaSpotsToFrame(personality, dest, FALSE);
        DrawSpindaSpotsToFrame(personality, dest, TRUE);
    }
}

u32 GetDecompressedDataSize(const u32 *ptr)
{
    const u8 *ptr8 = (const u8 *)ptr;
    return (ptr8[3] << 16) | (ptr8[2] << 8) | (ptr8[1]);
}

bool8 LoadCompressedSpriteSheetUsingHeap(const struct CompressedSpriteSheet* src)
{
    struct SpriteSheet dest;
    void* buffer;

    buffer = AllocZeroed(*((u32*)(&src->data[0])) >> 8);
    LZ77UnCompWram(src->data, buffer);

    dest.data = buffer;
    dest.size = src->size;
    dest.tag = src->tag;

    LoadSpriteSheet(&dest);
    Free(buffer);
    return FALSE;
}

bool8 LoadCompressedSpritePaletteUsingHeap(const struct CompressedSpritePalette *src)
{
    struct SpritePalette dest;
    void* buffer;

    buffer = AllocZeroed(*((u32*)(&src->data[0])) >> 8);
    LZ77UnCompWram(src->data, buffer);
    dest.data = buffer;
    dest.tag = src->tag;

    LoadSpritePalette(&dest);
    Free(buffer);
    return FALSE;
}
