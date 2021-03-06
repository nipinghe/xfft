#include"../include/xfft_patch.h"
#include"../include/xfft_macro.h"

static void __fft02x02_bk( xfft_kernel_t* const p, CUmodule module )
{
	xfft_create_kernel( &p[0], module, "d_fft2x2x"	 );
	xfft_create_kernel( &p[1], module, "d_ifft2x2x"  );
	xfft_kernel_sao( &p[0], XFFT_AM_P_I );
	xfft_kernel_sao( &p[1], XFFT_AM_P_I );
}
static void __fft04x04_bk( xfft_kernel_t* const p, CUmodule module )
{
	xfft_create_kernel( &p[0], module, "d_fft4x4x"  );
	xfft_create_kernel( &p[1], module, "d_ifft4x4x" );
	xfft_kernel_sao( &p[0], XFFT_AM_P_I );
	xfft_kernel_sao( &p[1], XFFT_AM_P_I );
}
static void __fft08x08_bk( xfft_kernel_t* const p, CUmodule module )
{
	xfft_create_kernel( &p[0], module, "d_fft8x8x"  );
	xfft_create_kernel( &p[1], module, "d_ifft8x8x" );
	xfft_kernel_sao( &p[0], XFFT_AM_P_I );
	xfft_kernel_sao( &p[1], XFFT_AM_P_I );
}
static void __fft16x16_bk( xfft_kernel_t* const p, CUmodule module )
{
	xfft_create_kernel( &p[0], module, "d_fft16x16x"  );
	xfft_create_kernel( &p[1], module, "d_ifft16x16x" );
	xfft_kernel_sao( &p[0], XFFT_AM_P_I );
	xfft_kernel_sao( &p[1], XFFT_AM_P_I );
}
static void __fft32x32_bk( xfft_kernel_t* const p, CUmodule module )
{
	xfft_create_kernel( &p[0], module, "d_fft32x32x"  );
	xfft_create_kernel( &p[1], module, "d_ifft32x32x" );
	xfft_kernel_sao( &p[0], XFFT_AM_P_P );
	xfft_kernel_sao( &p[1], XFFT_AM_P_P );
}
static void __fft64x64_bk( xfft_kernel_t* const p, CUmodule module )
{
	xfft_create_kernel( &p[0], module, "d_fft64x64x"  );
	xfft_create_kernel( &p[1], module, "d_ifft64x64x" );
	xfft_kernel_sao( &p[0], XFFT_AM_P_P );
	xfft_kernel_sao( &p[1], XFFT_AM_P_P );
}
static void (*p_bk[])( xfft_kernel_t*, CUmodule )=
{
	__fft02x02_bk,
	__fft04x04_bk,
	__fft08x08_bk,
	__fft16x16_bk,
	__fft32x32_bk,
	__fft64x64_bk
};
static void __sgl02( xfft_kernel_t* const p, int bat )
{
	unsigned int nb, nt;
	nb=(bat<128)?1:(bat+127)>>7;
	nt=(bat<128)?nAFFI(bat,32):128;
	xfft_kernel_sgl( &p[0], nb, 1 );
	xfft_kernel_sbl( &p[0], nt, 1 );
	xfft_kernel_sgl( &p[1], nb, 1 );
	xfft_kernel_sbl( &p[1], nt, 1 );
}
static void __sgl04( xfft_kernel_t* const p, int bat )
{
	__sgl02( p, bat );
}
static void __sgl08( xfft_kernel_t* const p, int bat )
{
	unsigned int nb, dy, smemnb;
	nb=(bat<16)?1:(bat+15)>>4;
	dy=(bat<16)?(nAFFI(bat<<3,32)>>3):16;
	smemnb=dy*72*sizeof(float);
	xfft_kernel_sgl( &p[0], nb, 1 );
	xfft_kernel_sbl( &p[0], 8, dy );
	p[0].smemnb=smemnb;
	xfft_kernel_sgl( &p[1], nb, 1 );
	xfft_kernel_sbl( &p[1], 8, dy );
	p[1].smemnb=smemnb;
}
static void __sgl16( xfft_kernel_t* const p, int bat )
{
	unsigned int nb, dy, smemnb;
	nb=(bat<16)?1:(bat+15)>>4;
	dy=(bat<16)?(nAFFI(bat<<4,32)>>4):16;
	smemnb=dy*272*sizeof(float);
	xfft_kernel_sgl( &p[0], nb, 1  );
	xfft_kernel_sbl( &p[0], 16, dy );
	p[0].smemnb=smemnb;
	xfft_kernel_sgl( &p[1], nb, 1  );
	xfft_kernel_sbl( &p[1], 16, dy );
	p[1].smemnb=smemnb;
}
static void __sgl32( xfft_kernel_t* const p, int bat )
{
	xfft_kernel_sgl( &p[0], bat,1 );
	xfft_kernel_sbl( &p[0], 32, 4 );
	xfft_kernel_sgl( &p[1], bat,1 );
	xfft_kernel_sbl( &p[1], 32, 4 );
}
static void __sgl64( xfft_kernel_t* const p, int bat )
{
	xfft_kernel_sgl( &p[0], bat,1 );
	xfft_kernel_sbl( &p[0], 64, 8 );
	xfft_kernel_sgl( &p[1], bat,1 );
	xfft_kernel_sbl( &p[1], 64, 8 );
}
static void (*p_sgl[])( xfft_kernel_t* const, int )=
{
	__sgl02,
	__sgl04,
	__sgl08,
	__sgl16,
	__sgl32,
	__sgl64
};
static __forceinline void fft2d_patch_bk( xfft_kernel_t* const p, CUmodule module, int i )
{
	p_bk[i]( p, module );
}
static __forceinline void fft2d_patch_sgl( xfft_kernel_t* const p, int bat, int i )
{
	p_sgl[i]( p, bat );
}
void fft2d_patch_bki( xfft_kernel_t* const p, CUmodule module, CUdeviceptr d_RF, int bat, int e )
{
	fft2d_patch_bk( p, module, e );
	fft2d_patch_sgl( p, bat, e );
	if(e>3){
		xfft_kernel_sep_ptr( &p[0], 1, d_RF );
		xfft_kernel_sep_ptr( &p[1], 1, d_RF );
	} else {
		xfft_kernel_sep_i32( &p[0], 1, bat );
		xfft_kernel_sep_i32( &p[1], 1, bat );
	}
}