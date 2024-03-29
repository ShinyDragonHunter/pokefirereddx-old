static const u16 sSpeciesIdToCryId[SPECIES_COUNT] =
{
    [SPECIES_NONE] = CRY_BULBASAUR,
    [SPECIES_BULBASAUR] = CRY_BULBASAUR,
    [SPECIES_IVYSAUR] = CRY_IVYSAUR,
    [SPECIES_VENUSAUR] = CRY_VENUSAUR,
    [SPECIES_CHARMANDER] = CRY_CHARMANDER,
    [SPECIES_CHARMELEON] = CRY_CHARMELEON,
    [SPECIES_CHARIZARD] = CRY_CHARIZARD,
    [SPECIES_SQUIRTLE] = CRY_SQUIRTLE,
    [SPECIES_WARTORTLE] = CRY_WARTORTLE,
    [SPECIES_BLASTOISE] = CRY_BLASTOISE,
    [SPECIES_CATERPIE] = CRY_CATERPIE,
    [SPECIES_METAPOD] = CRY_METAPOD,
    [SPECIES_BUTTERFREE] = CRY_BUTTERFREE,
    [SPECIES_WEEDLE] = CRY_WEEDLE,
    [SPECIES_KAKUNA] = CRY_KAKUNA,
    [SPECIES_BEEDRILL] = CRY_BEEDRILL,
    [SPECIES_PIDGEY] = CRY_PIDGEY,
    [SPECIES_PIDGEOTTO] = CRY_PIDGEOTTO,
    [SPECIES_PIDGEOT] = CRY_PIDGEOT,
    [SPECIES_RATTATA] = CRY_RATTATA,
    [SPECIES_RATICATE] = CRY_RATICATE,
    [SPECIES_SPEAROW] = CRY_SPEAROW,
    [SPECIES_FEAROW] = CRY_FEAROW,
    [SPECIES_EKANS] = CRY_EKANS,
    [SPECIES_ARBOK] = CRY_ARBOK,
    [SPECIES_PIKACHU] = CRY_PIKACHU,
    [SPECIES_RAICHU] = CRY_RAICHU,
    [SPECIES_SANDSHREW] = CRY_SANDSHREW,
    [SPECIES_SANDSLASH] = CRY_SANDSLASH,
    [SPECIES_NIDORAN_F] = CRY_NIDORAN_F,
    [SPECIES_NIDORINA] = CRY_NIDORINA,
    [SPECIES_NIDOQUEEN] = CRY_NIDOQUEEN,
    [SPECIES_NIDORAN_M] = CRY_NIDORAN_M,
    [SPECIES_NIDORINO] = CRY_NIDORINO,
    [SPECIES_NIDOKING] = CRY_NIDOKING,
    [SPECIES_CLEFAIRY] = CRY_CLEFAIRY,
    [SPECIES_CLEFABLE] = CRY_CLEFABLE,
    [SPECIES_VULPIX] = CRY_VULPIX,
    [SPECIES_NINETALES] = CRY_NINETALES,
    [SPECIES_JIGGLYPUFF] = CRY_JIGGLYPUFF,
    [SPECIES_WIGGLYTUFF] = CRY_WIGGLYTUFF,
    [SPECIES_ZUBAT] = CRY_ZUBAT,
    [SPECIES_GOLBAT] = CRY_GOLBAT,
    [SPECIES_ODDISH] = CRY_ODDISH,
    [SPECIES_GLOOM] = CRY_GLOOM,
    [SPECIES_VILEPLUME] = CRY_VILEPLUME,
    [SPECIES_PARAS] = CRY_PARAS,
    [SPECIES_PARASECT] = CRY_PARASECT,
    [SPECIES_VENONAT] = CRY_VENONAT,
    [SPECIES_VENOMOTH] = CRY_VENOMOTH,
    [SPECIES_DIGLETT] = CRY_DIGLETT,
    [SPECIES_DUGTRIO] = CRY_DUGTRIO,
    [SPECIES_MEOWTH] = CRY_MEOWTH,
    [SPECIES_PERSIAN] = CRY_PERSIAN,
    [SPECIES_PSYDUCK] = CRY_PSYDUCK,
    [SPECIES_GOLDUCK] = CRY_GOLDUCK,
    [SPECIES_MANKEY] = CRY_MANKEY,
    [SPECIES_PRIMEAPE] = CRY_PRIMEAPE,
    [SPECIES_GROWLITHE] = CRY_GROWLITHE,
    [SPECIES_ARCANINE] = CRY_ARCANINE,
    [SPECIES_POLIWAG] = CRY_POLIWAG,
    [SPECIES_POLIWHIRL] = CRY_POLIWHIRL,
    [SPECIES_POLIWRATH] = CRY_POLIWRATH,
    [SPECIES_ABRA] = CRY_ABRA,
    [SPECIES_KADABRA] = CRY_KADABRA,
    [SPECIES_ALAKAZAM] = CRY_ALAKAZAM,
    [SPECIES_MACHOP] = CRY_MACHOP,
    [SPECIES_MACHOKE] = CRY_MACHOKE,
    [SPECIES_MACHAMP] = CRY_MACHAMP,
    [SPECIES_BELLSPROUT] = CRY_BELLSPROUT,
    [SPECIES_WEEPINBELL] = CRY_WEEPINBELL,
    [SPECIES_VICTREEBEL] = CRY_VICTREEBEL,
    [SPECIES_TENTACOOL]  = CRY_TENTACOOL,
    [SPECIES_TENTACRUEL] = CRY_TENTACRUEL,
    [SPECIES_GEODUDE] = CRY_GEODUDE,
    [SPECIES_GRAVELER] = CRY_GRAVELER,
    [SPECIES_GOLEM] = CRY_GOLEM,
    [SPECIES_PONYTA] = CRY_PONYTA,
    [SPECIES_RAPIDASH] = CRY_RAPIDASH,
    [SPECIES_SLOWPOKE] = CRY_SLOWPOKE,
    [SPECIES_SLOWBRO] = CRY_SLOWBRO,
    [SPECIES_MAGNEMITE] = CRY_MAGNEMITE,
    [SPECIES_MAGNETON] = CRY_MAGNETON,
    [SPECIES_FARFETCHD] = CRY_FARFETCHD,
    [SPECIES_DODUO] = CRY_DODUO,
    [SPECIES_DODRIO] = CRY_DODRIO,
    [SPECIES_SEEL] = CRY_SEEL,
    [SPECIES_DEWGONG] = CRY_DEWGONG,
    [SPECIES_GRIMER] = CRY_GRIMER,
    [SPECIES_MUK] = CRY_MUK,
    [SPECIES_SHELLDER] = CRY_SHELLDER,
    [SPECIES_CLOYSTER] = CRY_CLOYSTER,
    [SPECIES_GASTLY] = CRY_GASTLY,
    [SPECIES_HAUNTER] = CRY_HAUNTER,
    [SPECIES_GENGAR] = CRY_GENGAR,
    [SPECIES_ONIX] = CRY_ONIX,
    [SPECIES_DROWZEE] = CRY_DROWZEE,
    [SPECIES_HYPNO] = CRY_HYPNO,
    [SPECIES_KRABBY] = CRY_KRABBY,
    [SPECIES_KINGLER] = CRY_KINGLER,
    [SPECIES_VOLTORB] = CRY_VOLTORB,
    [SPECIES_ELECTRODE] = CRY_ELECTRODE,
    [SPECIES_EXEGGCUTE] = CRY_EXEGGCUTE,
    [SPECIES_EXEGGUTOR] = CRY_EXEGGUTOR,
    [SPECIES_CUBONE] = CRY_CUBONE,
    [SPECIES_MAROWAK] = CRY_MAROWAK,
    [SPECIES_HITMONLEE] = CRY_HITMONLEE,
    [SPECIES_HITMONCHAN] = CRY_HITMONCHAN,
    [SPECIES_LICKITUNG] = CRY_LICKITUNG,
    [SPECIES_KOFFING] = CRY_KOFFING,
    [SPECIES_WEEZING] = CRY_WEEZING,
    [SPECIES_RHYHORN] = CRY_RHYHORN,
    [SPECIES_RHYDON] = CRY_RHYDON,
    [SPECIES_CHANSEY] = CRY_CHANSEY,
    [SPECIES_TANGELA] = CRY_TANGELA,
    [SPECIES_KANGASKHAN] = CRY_KANGASKHAN,
    [SPECIES_HORSEA] = CRY_HORSEA,
    [SPECIES_SEADRA] = CRY_SEADRA,
    [SPECIES_GOLDEEN] = CRY_GOLDEEN,
    [SPECIES_SEAKING] = CRY_SEAKING,
    [SPECIES_STARYU] = CRY_STARYU,
    [SPECIES_STARMIE] = CRY_STARMIE,
    [SPECIES_MR_MIME] = CRY_MR_MIME,
    [SPECIES_SCYTHER] = CRY_SCYTHER,
    [SPECIES_JYNX] = CRY_JYNX,
    [SPECIES_ELECTABUZZ] = CRY_ELECTABUZZ,
    [SPECIES_MAGMAR] = CRY_MAGMAR,
    [SPECIES_PINSIR] = CRY_PINSIR,
    [SPECIES_TAUROS] = CRY_TAUROS,
    [SPECIES_MAGIKARP] = CRY_MAGIKARP,
    [SPECIES_GYARADOS] = CRY_GYARADOS,
    [SPECIES_LAPRAS] = CRY_LAPRAS,
    [SPECIES_DITTO] = CRY_DITTO,
    [SPECIES_EEVEE] = CRY_EEVEE,
    [SPECIES_VAPOREON] = CRY_VAPOREON,
    [SPECIES_JOLTEON] = CRY_JOLTEON,
    [SPECIES_FLAREON] = CRY_FLAREON,
    [SPECIES_PORYGON] = CRY_PORYGON,
    [SPECIES_OMANYTE] = CRY_OMANYTE,
    [SPECIES_OMASTAR] = CRY_OMASTAR,
    [SPECIES_KABUTO] = CRY_KABUTO,
    [SPECIES_KABUTOPS] = CRY_KABUTOPS,
    [SPECIES_AERODACTYL] = CRY_AERODACTYL,
    [SPECIES_SNORLAX] = CRY_SNORLAX,
    [SPECIES_ARTICUNO] = CRY_ARTICUNO,
    [SPECIES_ZAPDOS] = CRY_ZAPDOS,
    [SPECIES_MOLTRES] = CRY_MOLTRES,
    [SPECIES_DRATINI] = CRY_DRATINI,
    [SPECIES_DRAGONAIR] = CRY_DRAGONAIR,
    [SPECIES_DRAGONITE] = CRY_DRAGONITE,
    [SPECIES_MEWTWO] = CRY_MEWTWO,
    [SPECIES_MEW] = CRY_MEW,
    [SPECIES_CHIKORITA] = CRY_CHIKORITA,
    [SPECIES_BAYLEEF]  = CRY_BAYLEEF,
    [SPECIES_MEGANIUM] = CRY_MEGANIUM,
    [SPECIES_CYNDAQUIL] = CRY_CYNDAQUIL,
    [SPECIES_QUILAVA] = CRY_QUILAVA,
    [SPECIES_TYPHLOSION] = CRY_TYPHLOSION,
    [SPECIES_TOTODILE] = CRY_TOTODILE,
    [SPECIES_CROCONAW] = CRY_CROCONAW,
    [SPECIES_FERALIGATR] = CRY_FERALIGATR,
    [SPECIES_SENTRET] = CRY_SENTRET,
    [SPECIES_FURRET] = CRY_FURRET,
    [SPECIES_HOOTHOOT] = CRY_HOOTHOOT,
    [SPECIES_NOCTOWL] = CRY_NOCTOWL,
    [SPECIES_LEDYBA] = CRY_LEDYBA,
    [SPECIES_LEDIAN] = CRY_LEDIAN,
    [SPECIES_SPINARAK] = CRY_SPINARAK,
    [SPECIES_ARIADOS] = CRY_ARIADOS,
    [SPECIES_CROBAT] = CRY_CROBAT,
    [SPECIES_CHINCHOU] = CRY_CHINCHOU,
    [SPECIES_LANTURN] = CRY_LANTURN,
    [SPECIES_PICHU] = CRY_PICHU,
    [SPECIES_CLEFFA] = CRY_CLEFFA,
    [SPECIES_IGGLYBUFF] = CRY_IGGLYBUFF,
    [SPECIES_TOGEPI] = CRY_TOGEPI,
    [SPECIES_TOGETIC] = CRY_TOGETIC,
    [SPECIES_NATU] = CRY_NATU,
    [SPECIES_XATU] = CRY_XATU,
    [SPECIES_MAREEP] = CRY_MAREEP,
    [SPECIES_FLAAFFY] = CRY_FLAAFFY,
    [SPECIES_AMPHAROS] = CRY_AMPHAROS,
    [SPECIES_BELLOSSOM] = CRY_BELLOSSOM,
    [SPECIES_MARILL] = CRY_MARILL,
    [SPECIES_AZUMARILL] = CRY_AZUMARILL,
    [SPECIES_SUDOWOODO] = CRY_SUDOWOODO,
    [SPECIES_POLITOED] = CRY_POLITOED,
    [SPECIES_HOPPIP] = CRY_HOPPIP,
    [SPECIES_SKIPLOOM] = CRY_SKIPLOOM,
    [SPECIES_JUMPLUFF] = CRY_JUMPLUFF,
    [SPECIES_AIPOM] = CRY_AIPOM,
    [SPECIES_SUNKERN] = CRY_SUNKERN,
    [SPECIES_SUNFLORA] = CRY_SUNFLORA,
    [SPECIES_YANMA] = CRY_YANMA,
    [SPECIES_WOOPER] = CRY_WOOPER,
    [SPECIES_QUAGSIRE] = CRY_QUAGSIRE,
    [SPECIES_ESPEON] = CRY_ESPEON,
    [SPECIES_UMBREON] = CRY_UMBREON,
    [SPECIES_MURKROW] = CRY_MURKROW,
    [SPECIES_SLOWKING] = CRY_SLOWKING,
    [SPECIES_MISDREAVUS] = CRY_MISDREAVUS,
    [SPECIES_UNOWN] = CRY_UNOWN,
    [SPECIES_WOBBUFFET] = CRY_WOBBUFFET,
    [SPECIES_GIRAFARIG] = CRY_GIRAFARIG,
    [SPECIES_PINECO] = CRY_PINECO,
    [SPECIES_FORRETRESS] = CRY_FORRETRESS,
    [SPECIES_DUNSPARCE] = CRY_DUNSPARCE,
    [SPECIES_GLIGAR] = CRY_GLIGAR,
    [SPECIES_STEELIX] = CRY_STEELIX,
    [SPECIES_SNUBBULL] = CRY_SNUBBULL,
    [SPECIES_GRANBULL] = CRY_GRANBULL,
    [SPECIES_QWILFISH] = CRY_QWILFISH,
    [SPECIES_SCIZOR] = CRY_SCIZOR,
    [SPECIES_SHUCKLE] = CRY_SHUCKLE,
    [SPECIES_HERACROSS] = CRY_HERACROSS,
    [SPECIES_SNEASEL] = CRY_SNEASEL,
    [SPECIES_TEDDIURSA] = CRY_TEDDIURSA,
    [SPECIES_URSARING] = CRY_URSARING,
    [SPECIES_SLUGMA] = CRY_SLUGMA,
    [SPECIES_MAGCARGO] = CRY_MAGCARGO,
    [SPECIES_SWINUB] = CRY_SWINUB,
    [SPECIES_PILOSWINE] = CRY_PILOSWINE,
    [SPECIES_CORSOLA] = CRY_CORSOLA,
    [SPECIES_REMORAID] = CRY_REMORAID,
    [SPECIES_OCTILLERY] = CRY_OCTILLERY,
    [SPECIES_DELIBIRD] = CRY_DELIBIRD,
    [SPECIES_MANTINE] = CRY_MANTINE,
    [SPECIES_SKARMORY] = CRY_SKARMORY,
    [SPECIES_HOUNDOUR] = CRY_HOUNDOUR,
    [SPECIES_HOUNDOOM] = CRY_HOUNDOOM,
    [SPECIES_KINGDRA] = CRY_KINGDRA,
    [SPECIES_PHANPY] = CRY_PHANPY,
    [SPECIES_DONPHAN] = CRY_DONPHAN,
    [SPECIES_PORYGON2] = CRY_PORYGON2,
    [SPECIES_STANTLER] = CRY_STANTLER,
    [SPECIES_SMEARGLE] = CRY_SMEARGLE,
    [SPECIES_TYROGUE] = CRY_TYROGUE,
    [SPECIES_HITMONTOP] = CRY_HITMONTOP,
    [SPECIES_SMOOCHUM] = CRY_SMOOCHUM,
    [SPECIES_ELEKID] = CRY_ELEKID,
    [SPECIES_MAGBY] = CRY_MAGBY,
    [SPECIES_MILTANK] = CRY_MILTANK,
    [SPECIES_BLISSEY] = CRY_BLISSEY,
    [SPECIES_RAIKOU] = CRY_RAIKOU,
    [SPECIES_ENTEI] = CRY_ENTEI,
    [SPECIES_SUICUNE] = CRY_SUICUNE,
    [SPECIES_LARVITAR] = CRY_LARVITAR,
    [SPECIES_PUPITAR] = CRY_PUPITAR,
    [SPECIES_TYRANITAR] = CRY_TYRANITAR,
    [SPECIES_LUGIA] = CRY_LUGIA,
    [SPECIES_HO_OH] = CRY_HO_OH,
    [SPECIES_CELEBI] = CRY_CELEBI,
    [SPECIES_OLD_UNOWN_B] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_C] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_D] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_E] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_F] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_G] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_H] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_I] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_J] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_K] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_L] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_M] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_N] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_O] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_P] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_Q] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_R] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_S] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_T] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_U] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_V] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_W] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_X] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_Y] = CRY_UNOWN,
    [SPECIES_OLD_UNOWN_Z] = CRY_UNOWN,
    [SPECIES_TREECKO] = CRY_TREECKO,
    [SPECIES_GROVYLE] = CRY_GROVYLE,
    [SPECIES_SCEPTILE] = CRY_SCEPTILE,
    [SPECIES_TORCHIC] = CRY_TORCHIC,
    [SPECIES_COMBUSKEN] = CRY_COMBUSKEN,
    [SPECIES_BLAZIKEN] = CRY_BLAZIKEN,
    [SPECIES_MUDKIP] = CRY_MUDKIP,
    [SPECIES_MARSHTOMP] = CRY_MARSHTOMP,
    [SPECIES_SWAMPERT] = CRY_SWAMPERT,
    [SPECIES_POOCHYENA] = CRY_POOCHYENA,
    [SPECIES_MIGHTYENA] = CRY_MIGHTYENA,
    [SPECIES_ZIGZAGOON] = CRY_ZIGZAGOON,
    [SPECIES_LINOONE] = CRY_LINOONE,
    [SPECIES_WURMPLE] = CRY_WURMPLE,
    [SPECIES_SILCOON] = CRY_SILCOON,
    [SPECIES_BEAUTIFLY] = CRY_BEAUTIFLY,
    [SPECIES_CASCOON] = CRY_CASCOON,
    [SPECIES_DUSTOX] = CRY_DUSTOX,
    [SPECIES_LOTAD] = CRY_LOTAD,
    [SPECIES_LOMBRE] = CRY_LOMBRE,
    [SPECIES_LUDICOLO] = CRY_LUDICOLO,
    [SPECIES_SEEDOT] = CRY_SEEDOT,
    [SPECIES_NUZLEAF] = CRY_NUZLEAF,
    [SPECIES_SHIFTRY] = CRY_SHIFTRY,
    [SPECIES_NINCADA] = CRY_NINCADA,
    [SPECIES_NINJASK] = CRY_NINJASK,
    [SPECIES_SHEDINJA] = CRY_SHEDINJA,
    [SPECIES_TAILLOW] = CRY_TAILLOW,
    [SPECIES_SWELLOW] = CRY_SWELLOW,
    [SPECIES_SHROOMISH] = CRY_SHROOMISH,
    [SPECIES_BRELOOM] = CRY_BRELOOM,
    [SPECIES_SPINDA] = CRY_SPINDA,
    [SPECIES_WINGULL] = CRY_WINGULL,
    [SPECIES_PELIPPER] = CRY_PELIPPER,
    [SPECIES_SURSKIT] = CRY_SURSKIT,
    [SPECIES_MASQUERAIN] = CRY_MASQUERAIN,
    [SPECIES_WAILMER] = CRY_WAILMER,
    [SPECIES_WAILORD] = CRY_WAILORD,
    [SPECIES_SKITTY] = CRY_SKITTY,
    [SPECIES_DELCATTY] = CRY_DELCATTY,
    [SPECIES_KECLEON] = CRY_KECLEON,
    [SPECIES_BALTOY] = CRY_BALTOY,
    [SPECIES_CLAYDOL] = CRY_CLAYDOL,
    [SPECIES_NOSEPASS] = CRY_NOSEPASS,
    [SPECIES_TORKOAL] = CRY_TORKOAL,
    [SPECIES_SABLEYE] = CRY_SABLEYE,
    [SPECIES_BARBOACH] = CRY_BARBOACH,
    [SPECIES_WHISCASH] = CRY_WHISCASH,
    [SPECIES_LUVDISC] = CRY_LUVDISC,
    [SPECIES_CORPHISH] = CRY_CORPHISH,
    [SPECIES_CRAWDAUNT] = CRY_CRAWDAUNT,
    [SPECIES_FEEBAS] = CRY_FEEBAS,
    [SPECIES_MILOTIC] = CRY_MILOTIC,
    [SPECIES_CARVANHA] = CRY_CARVANHA,
    [SPECIES_SHARPEDO] = CRY_SHARPEDO,
    [SPECIES_TRAPINCH] = CRY_TRAPINCH,
    [SPECIES_VIBRAVA] = CRY_VIBRAVA,
    [SPECIES_FLYGON] = CRY_FLYGON,
    [SPECIES_MAKUHITA] = CRY_MAKUHITA,
    [SPECIES_HARIYAMA] = CRY_HARIYAMA,
    [SPECIES_ELECTRIKE] = CRY_ELECTRIKE,
    [SPECIES_MANECTRIC] = CRY_MANECTRIC,
    [SPECIES_NUMEL] = CRY_NUMEL,
    [SPECIES_CAMERUPT] = CRY_CAMERUPT,
    [SPECIES_SPHEAL] = CRY_SPHEAL,
    [SPECIES_SEALEO] = CRY_SEALEO,
    [SPECIES_WALREIN] = CRY_WALREIN,
    [SPECIES_CACNEA] = CRY_CACNEA,
    [SPECIES_CACTURNE] = CRY_CACTURNE,
    [SPECIES_SNORUNT] = CRY_SNORUNT,
    [SPECIES_GLALIE] = CRY_GLALIE,
    [SPECIES_LUNATONE] = CRY_LUNATONE,
    [SPECIES_SOLROCK] = CRY_SOLROCK,
    [SPECIES_AZURILL] = CRY_AZURILL,
    [SPECIES_SPOINK] = CRY_SPOINK,
    [SPECIES_GRUMPIG] = CRY_GRUMPIG,
    [SPECIES_PLUSLE] = CRY_PLUSLE,
    [SPECIES_MINUN] = CRY_MINUN,
    [SPECIES_MAWILE] = CRY_MAWILE,
    [SPECIES_MEDITITE] = CRY_MEDITITE,
    [SPECIES_MEDICHAM] = CRY_MEDICHAM,
    [SPECIES_SWABLU] = CRY_SWABLU,
    [SPECIES_ALTARIA] = CRY_ALTARIA,
    [SPECIES_WYNAUT] = CRY_WYNAUT,
    [SPECIES_DUSKULL] = CRY_DUSKULL,
    [SPECIES_DUSCLOPS] = CRY_DUSCLOPS,
    [SPECIES_ROSELIA] = CRY_ROSELIA,
    [SPECIES_SLAKOTH] = CRY_SLAKOTH,
    [SPECIES_VIGOROTH] = CRY_VIGOROTH,
    [SPECIES_SLAKING] = CRY_SLAKING,
    [SPECIES_GULPIN] = CRY_GULPIN,
    [SPECIES_SWALOT] = CRY_SWALOT,
    [SPECIES_TROPIUS] = CRY_TROPIUS,
    [SPECIES_WHISMUR] = CRY_WHISMUR,
    [SPECIES_LOUDRED] = CRY_LOUDRED,
    [SPECIES_EXPLOUD] = CRY_EXPLOUD,
    [SPECIES_CLAMPERL] = CRY_CLAMPERL,
    [SPECIES_HUNTAIL] = CRY_HUNTAIL,
    [SPECIES_GOREBYSS] = CRY_GOREBYSS,
    [SPECIES_ABSOL] = CRY_ABSOL,
    [SPECIES_SHUPPET] = CRY_SHUPPET,
    [SPECIES_BANETTE] = CRY_BANETTE,
    [SPECIES_SEVIPER] = CRY_SEVIPER,
    [SPECIES_ZANGOOSE] = CRY_ZANGOOSE,
    [SPECIES_RELICANTH] = CRY_RELICANTH,
    [SPECIES_ARON] = CRY_ARON,
    [SPECIES_LAIRON] = CRY_LAIRON,
    [SPECIES_AGGRON] = CRY_AGGRON,
    [SPECIES_CASTFORM] = CRY_CASTFORM,
    [SPECIES_VOLBEAT] = CRY_VOLBEAT,
    [SPECIES_ILLUMISE] = CRY_ILLUMISE,
    [SPECIES_LILEEP] = CRY_LILEEP,
    [SPECIES_CRADILY] = CRY_CRADILY,
    [SPECIES_ANORITH] = CRY_ANORITH,
    [SPECIES_ARMALDO] = CRY_ARMALDO,
    [SPECIES_RALTS] = CRY_RALTS,
    [SPECIES_KIRLIA] = CRY_KIRLIA,
    [SPECIES_GARDEVOIR] = CRY_GARDEVOIR,
    [SPECIES_BAGON] = CRY_BAGON,
    [SPECIES_SHELGON] = CRY_SHELGON,
    [SPECIES_SALAMENCE] = CRY_SALAMENCE,
    [SPECIES_BELDUM] = CRY_BELDUM,
    [SPECIES_METANG] = CRY_METANG,
    [SPECIES_METAGROSS] = CRY_METAGROSS,
    [SPECIES_REGIROCK] = CRY_REGIROCK,
    [SPECIES_REGICE] = CRY_REGICE,
    [SPECIES_REGISTEEL] = CRY_REGISTEEL,
    [SPECIES_KYOGRE] = CRY_KYOGRE,
    [SPECIES_GROUDON] = CRY_GROUDON,
    [SPECIES_RAYQUAZA] = CRY_RAYQUAZA,
    [SPECIES_LATIAS] = CRY_LATIAS,
    [SPECIES_LATIOS] = CRY_LATIOS,
    [SPECIES_JIRACHI] = CRY_JIRACHI,
    [SPECIES_DEOXYS] = CRY_DEOXYS,
    [SPECIES_CHIMECHO] = CRY_CHIMECHO,
    // Pok�mon forms
    [SPECIES_RATTATA_ALOLAN] = CRY_RATTATA,
    [SPECIES_RATICATE_ALOLAN] = CRY_RATICATE,
    [SPECIES_RAICHU_ALOLAN] = CRY_RAICHU,
    [SPECIES_SANDSHREW_ALOLAN] = CRY_SANDSHREW,
    [SPECIES_SANDSLASH_ALOLAN] = CRY_SANDSLASH,
    [SPECIES_VULPIX_ALOLAN] = CRY_VULPIX,
    [SPECIES_NINETALES_ALOLAN] = CRY_NINETALES,
    [SPECIES_DIGLETT_ALOLAN] = CRY_DIGLETT,
    [SPECIES_DUGTRIO_ALOLAN] = CRY_DUGTRIO,
    [SPECIES_MEOWTH_ALOLAN] = CRY_MEOWTH,
    [SPECIES_PERSIAN_ALOLAN] = CRY_PERSIAN,
    [SPECIES_GEODUDE_ALOLAN] = CRY_GEODUDE,
    [SPECIES_GRAVELER_ALOLAN] = CRY_GRAVELER,
    [SPECIES_GOLEM_ALOLAN] = CRY_GOLEM,
    [SPECIES_PONYTA_GALARIAN] = CRY_PONYTA,
    [SPECIES_RAPIDASH_GALARIAN] = CRY_RAPIDASH,
    [SPECIES_SLOWPOKE_GALARIAN] = CRY_SLOWPOKE,
    [SPECIES_SLOWBRO_GALARIAN] = CRY_SLOWBRO,
    [SPECIES_GRIMER_ALOLAN] = CRY_GRIMER,
    [SPECIES_MUK_ALOLAN] = CRY_MUK,
    [SPECIES_EXEGGUTOR_ALOLAN] = CRY_EXEGGUTOR,
    [SPECIES_MAROWAK_ALOLAN] = CRY_MAROWAK,
    [SPECIES_WEEZING_GALARIAN] = CRY_WEEZING,
    [SPECIES_ARTICUNO_GALARIAN] = CRY_ARTICUNO,
    [SPECIES_ZAPDOS_GALARIAN] = CRY_ZAPDOS,
    [SPECIES_MOLTRES_GALARIAN] = CRY_MOLTRES,
    [SPECIES_MEWTWO_ARMORED] = CRY_MEWTWO,
    [SPECIES_SLOWKING_GALARIAN] = CRY_SLOWKING,
    [SPECIES_LUGIA_SHADOW] = CRY_LUGIA,
    [SPECIES_DEOXYS_SPEED] = CRY_DEOXYS,
    [SPECIES_DEOXYS_ATTACK] = CRY_DEOXYS,
    [SPECIES_DEOXYS_DEFENSE] = CRY_DEOXYS,
};
