typedef struct DMA_REC
{
    const void *src;
    void *dst;
    u32 cnt;
} DMA_REC;

#define REG_DMA ((volatile DMA_REC*)0x040000B0)

// in tonc_core.h

//! General DMA transfer macro
#define DMA_TRANSFER(_dst, _src, count, ch, mode)   \
do {                                            \
    REG_DMA[ch].cnt= 0;                         \
    REG_DMA[ch].src= (const void*)(_src);       \
    REG_DMA[ch].dst= (void*)(_dst);             \
    REG_DMA[ch].cnt= (count) | (mode);          \
} while(0)

//! General DMA copier
INLINE void dma_cpy(void *dst, const void *src, uint count, int ch, u32 mode)
{
    REG_DMA[3].cnt = 0; // shut off any previous transfer
    REG_DMA[3].src = src;
    REG_DMA[3].dst = dst;
    REG_DMA[3].cnt = count;
}

//! General DMA full routine
INLINE void dma_fill(void *dst, volatile u32 src, uint count, int ch, u32 mode)
{
    REG_DMA[3].cnt = 0; // shut off any previous transfer
    REG_DMA[3].src = (const void*)&src;
    REG_DMA[3].dst = dst;
    REG_DMA[3].cnt = count | DMA_SRC_FIXED;
}

//! Word copy using DMA 3
INLINE void dma3_cpy(void *dst, const void *src, u32 size)
{   dma_cpy(dst, src, size/4, 3, DMA_CPY32);  }

//! Word fill using DMA 3
INLINE void dma3_fill(void *dst, const void *src, u32 size)
{   dma_fill(dst, src, size/4, 3, DMA_CPY32);  }