	.include "asm/macros.inc"
	.include "constants/constants.inc"

	.section .rodata

	.align 2
gFont0LatinGlyphs::
	.incbin "graphics/fonts/font0.latfont"

	.align 2
gFont0LatinGlyphWidths::
	.include "graphics/fonts/font0_latin_widths.inc"

	.align 2
gFont1LatinGlyphs::
	.incbin "graphics/fonts/font1.latfont"

	.align 2
gFont1LatinGlyphWidths::
	.include "graphics/fonts/font1_latin_widths.inc"

	.align 2
gFont1JapaneseGlyphs::
	.incbin "graphics/fonts/font1.hwjpnfont"

	.align 2
gFont2LatinGlyphs::
	.incbin "graphics/fonts/font2.latfont"

	.align 2
gFont2LatinGlyphWidths::
	.include "graphics/fonts/font2_latin_widths.inc"

	.align 2
gFont2JapaneseGlyphs::
	.incbin "graphics/fonts/font2.fwjpnfont"

	.align 2
gFont2JapaneseGlyphWidths::
	.include "graphics/fonts/font2_japanese_widths.inc"

	.align 2
gFont3LatinGlyphs::
	.incbin "graphics/fonts/font3.latfont"

	.align 2
gFont3LatinGlyphWidths::
	.include "graphics/fonts/font3_latin_widths.inc"

	.align 2
gFont4LatinGlyphs::
	.incbin "graphics/fonts/font4.latfont"

	.align 2
gFont4LatinGlyphWidths::
	.include "graphics/fonts/font4_latin_widths.inc"

	.align 2
gFont4JapaneseGlyphs::
	.incbin "graphics/fonts/font4.hwjpnfont"

	.align 2
gFont5LatinGlyphs::
	.incbin "graphics/fonts/font5.latfont"

	.align 2
gFont5LatinGlyphWidths::
	.include "graphics/fonts/font5_latin_widths.inc"

	.align 2
gFont6LatinGlyphWidths::
	.include "graphics/fonts/font6_latin_widths.inc"
