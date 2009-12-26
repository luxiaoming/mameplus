INLINE UINT32 FBWRITE_16_RDEN_CVGD0_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD0_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD1_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD1_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD2_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD2_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD3_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDEN_CVGD3_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD0_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD0_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD1_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD1_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD2_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD2_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD3_NCOC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_16_RDNEN_CVGD3_COC(UINT16 *fb, UINT8 *hb, UINT32 r, UINT32 g, UINT32 b);

INLINE UINT32 FBWRITE_32_RDEN_CVGD0_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD0_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD1_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD1_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD2_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD2_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD3_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDEN_CVGD3_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD0_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD0_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD1_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD1_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD2_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD2_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD3_NCOC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);
INLINE UINT32 FBWRITE_32_RDNEN_CVGD3_COC(UINT32 *fb, UINT32 r, UINT32 g, UINT32 b);

static UINT32 (*rdp_fbwrite_16_func[16])(UINT16*, UINT8*, UINT32, UINT32, UINT32) =
{
	FBWRITE_16_RDNEN_CVGD0_NCOC,	FBWRITE_16_RDNEN_CVGD1_NCOC,	FBWRITE_16_RDNEN_CVGD2_NCOC,	FBWRITE_16_RDNEN_CVGD3_NCOC,
	FBWRITE_16_RDEN_CVGD0_NCOC,		FBWRITE_16_RDEN_CVGD1_NCOC,		FBWRITE_16_RDEN_CVGD2_NCOC,		FBWRITE_16_RDEN_CVGD3_NCOC,
	FBWRITE_16_RDNEN_CVGD0_COC,		FBWRITE_16_RDNEN_CVGD1_COC,		FBWRITE_16_RDNEN_CVGD2_COC,		FBWRITE_16_RDNEN_CVGD3_COC,
	FBWRITE_16_RDEN_CVGD0_COC,		FBWRITE_16_RDEN_CVGD1_COC,		FBWRITE_16_RDEN_CVGD2_COC,		FBWRITE_16_RDEN_CVGD3_COC,
};

static UINT32 (*rdp_fbwrite_32_func[16])(UINT32*, UINT32, UINT32, UINT32) =
{
	FBWRITE_32_RDNEN_CVGD0_NCOC,	FBWRITE_32_RDNEN_CVGD1_NCOC,	FBWRITE_32_RDNEN_CVGD2_NCOC,	FBWRITE_32_RDNEN_CVGD3_NCOC,
	FBWRITE_32_RDEN_CVGD0_NCOC,		FBWRITE_32_RDEN_CVGD1_NCOC,		FBWRITE_32_RDEN_CVGD2_NCOC,		FBWRITE_32_RDEN_CVGD3_NCOC,
	FBWRITE_32_RDNEN_CVGD0_COC,		FBWRITE_32_RDNEN_CVGD1_COC,		FBWRITE_32_RDNEN_CVGD2_COC,		FBWRITE_32_RDNEN_CVGD3_COC,
	FBWRITE_32_RDEN_CVGD0_COC,		FBWRITE_32_RDEN_CVGD1_COC,		FBWRITE_32_RDEN_CVGD2_COC,		FBWRITE_32_RDEN_CVGD3_COC,
};
