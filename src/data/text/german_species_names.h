const u8 gGermanSpeciesNames[SPECIES_COUNT][POKEMON_NAME_LENGTH + 1] = {
    [SPECIES_NONE] = _("??????????"),
    [SPECIES_BULBASAUR] = _("BISASAM"),
    [SPECIES_IVYSAUR] = _("BISAKNOSP"),
    [SPECIES_VENUSAUR] = _("BISAFLOR"),
    [SPECIES_CHARMANDER] = _("GLUMANDA"),
    [SPECIES_CHARMELEON] = _("GLUTEXO"),
    [SPECIES_CHARIZARD] = _("GLURAK"),
    [SPECIES_SQUIRTLE] = _("SCHIGGY"),
    [SPECIES_WARTORTLE] = _("SCHILLOK"),
    [SPECIES_BLASTOISE] = _("TURTOK"),
    [SPECIES_CATERPIE] = _("RAUPY"),
    [SPECIES_METAPOD] = _("SAFCON"),
    [SPECIES_BUTTERFREE] = _("SMETTBO"),
    [SPECIES_WEEDLE] = _("HORNLIU"),
    [SPECIES_KAKUNA] = _("KOKUNA"),
    [SPECIES_BEEDRILL] = _("BIBOR"),
    [SPECIES_PIDGEY] = _("TAUBSI"),
    [SPECIES_PIDGEOTTO] = _("TAUBOGA"),
    [SPECIES_PIDGEOT] = _("TAUBOSS"),
    [SPECIES_RATTATA] = _("RATTFRATZ"),
    [SPECIES_RATICATE] = _("RATTIKARL"),
    [SPECIES_SPEAROW] = _("HABITAK"),
    [SPECIES_FEAROW] = _("IBITAK"),
    [SPECIES_EKANS] = _("RETTAN"),
    [SPECIES_ARBOK] = _("ARBOK"),
    [SPECIES_PIKACHU] = _("PIKACHU"),
    [SPECIES_RAICHU] = _("RAICHU"),
    [SPECIES_SANDSHREW] = _("SANDAN"),
    [SPECIES_SANDSLASH] = _("SANDAMER"),
    [SPECIES_NIDORAN_F] = _("NIDORAN♀"),
    [SPECIES_NIDORINA] = _("NIDORINA"),
    [SPECIES_NIDOQUEEN] = _("NIDOQUEEN"),
    [SPECIES_NIDORAN_M] = _("NIDORAN♂"),
    [SPECIES_NIDORINO] = _("NIDORINO"),
    [SPECIES_NIDOKING] = _("NIDOKING"),
    [SPECIES_CLEFAIRY] = _("PIEPI"),
    [SPECIES_CLEFABLE] = _("PIXI"),
    [SPECIES_VULPIX] = _("VULPIX"),
    [SPECIES_NINETALES] = _("VULNONA"),
    [SPECIES_JIGGLYPUFF] = _("PUMMELUFF"),
    [SPECIES_WIGGLYTUFF] = _("KNUDDELUFF"),
    [SPECIES_ZUBAT] = _("ZUBAT"),
    [SPECIES_GOLBAT] = _("GOLBAT"),
    [SPECIES_ODDISH] = _("MYRAPLA"),
    [SPECIES_GLOOM] = _("DUFLOR"),
    [SPECIES_VILEPLUME] = _("GIFLOR"),
    [SPECIES_PARAS] = _("PARAS"),
    [SPECIES_PARASECT] = _("PARASEK"),
    [SPECIES_VENONAT] = _("BLUZUK"),
    [SPECIES_VENOMOTH] = _("OMOT"),
    [SPECIES_DIGLETT] = _("DIGDA"),
    [SPECIES_DUGTRIO] = _("DIGDRI"),
    [SPECIES_MEOWTH] = _("MAUZI"),
    [SPECIES_PERSIAN] = _("SNOBILIKAT"),
    [SPECIES_PSYDUCK] = _("ENTON"),
    [SPECIES_GOLDUCK] = _("ENTORON"),
    [SPECIES_MANKEY] = _("MENKI"),
    [SPECIES_PRIMEAPE] = _("RASAFF"),
    [SPECIES_GROWLITHE] = _("FUKANO"),
    [SPECIES_ARCANINE] = _("ARKANI"),
    [SPECIES_POLIWAG] = _("QUAPSEL"),
    [SPECIES_POLIWHIRL] = _("QUAPUTZI"),
    [SPECIES_POLIWRATH] = _("QUAPPO"),
    [SPECIES_ABRA] = _("ABRA"),
    [SPECIES_KADABRA] = _("KADABRA"),
    [SPECIES_ALAKAZAM] = _("SIMSALA"),
    [SPECIES_MACHOP] = _("MACHOLLO"),
    [SPECIES_MACHOKE] = _("MASCHOCK"),
    [SPECIES_MACHAMP] = _("MACHOMEI"),
    [SPECIES_BELLSPROUT] = _("KNOFENSA"),
    [SPECIES_WEEPINBELL] = _("ULTRIGARIA"),
    [SPECIES_VICTREEBEL] = _("SARZENIA"),
    [SPECIES_TENTACOOL] = _("TENTACHA"),
    [SPECIES_TENTACRUEL] = _("TENTOXA"),
    [SPECIES_GEODUDE] = _("KLEINSTEIN"),
    [SPECIES_GRAVELER] = _("GEOROK"),
    [SPECIES_GOLEM] = _("GEOWAZ"),
    [SPECIES_PONYTA] = _("PONITA"),
    [SPECIES_RAPIDASH] = _("GALLOPA"),
    [SPECIES_SLOWPOKE] = _("FLEGMON"),
    [SPECIES_SLOWBRO] = _("LAHMUS"),
    [SPECIES_MAGNEMITE] = _("MAGNETILO"),
    [SPECIES_MAGNETON] = _("MAGNETON"),
    [SPECIES_FARFETCHD] = _("PORENTA"),
    [SPECIES_DODUO] = _("DODU"),
    [SPECIES_DODRIO] = _("DODRI"),
    [SPECIES_SEEL] = _("JUROB"),
    [SPECIES_DEWGONG] = _("JUGONG"),
    [SPECIES_GRIMER] = _("SLEIMA"),
    [SPECIES_MUK] = _("SLEIMOK"),
    [SPECIES_SHELLDER] = _("MUSCHAS"),
    [SPECIES_CLOYSTER] = _("AUSTOS"),
    [SPECIES_GASTLY] = _("NEBULAK"),
    [SPECIES_HAUNTER] = _("ALPOLLO"),
    [SPECIES_GENGAR] = _("GENGAR"),
    [SPECIES_ONIX] = _("ONIX"),
    [SPECIES_DROWZEE] = _("TRAUMATO"),
    [SPECIES_HYPNO] = _("HYPNO"),
    [SPECIES_KRABBY] = _("KRABBY"),
    [SPECIES_KINGLER] = _("KINGLER"),
    [SPECIES_VOLTORB] = _("VOLTOBAL"),
    [SPECIES_ELECTRODE] = _("LEKTROBAL"),
    [SPECIES_EXEGGCUTE] = _("OWEI"),
    [SPECIES_EXEGGUTOR] = _("KOKOWEI"),
    [SPECIES_CUBONE] = _("TRAGOSSO"),
    [SPECIES_MAROWAK] = _("KNOGGA"),
    [SPECIES_HITMONLEE] = _("KICKLEE"),
    [SPECIES_HITMONCHAN] = _("NOCKCHAN"),
    [SPECIES_LICKITUNG] = _("SCHLURP"),
    [SPECIES_KOFFING] = _("SMOGON"),
    [SPECIES_WEEZING] = _("SMOGMOG"),
    [SPECIES_RHYHORN] = _("RIHORN"),
    [SPECIES_RHYDON] = _("RIZEROS"),
    [SPECIES_CHANSEY] = _("CHANEIRA"),
    [SPECIES_TANGELA] = _("TANGELA"),
    [SPECIES_KANGASKHAN] = _("KANGAMA"),
    [SPECIES_HORSEA] = _("SEEPER"),
    [SPECIES_SEADRA] = _("SEEMON"),
    [SPECIES_GOLDEEN] = _("GOLDINI"),
    [SPECIES_SEAKING] = _("GOLKING"),
    [SPECIES_STARYU] = _("STERNDU"),
    [SPECIES_STARMIE] = _("STARMIE"),
    [SPECIES_MR_MIME] = _("PANTIMOS"),
    [SPECIES_SCYTHER] = _("SICHLOR"),
    [SPECIES_JYNX] = _("ROSSANA"),
    [SPECIES_ELECTABUZZ] = _("ELEKTEK"),
    [SPECIES_MAGMAR] = _("MAGMAR"),
    [SPECIES_PINSIR] = _("PINSIR"),
    [SPECIES_TAUROS] = _("TAUROS"),
    [SPECIES_MAGIKARP] = _("KARPADOR"),
    [SPECIES_GYARADOS] = _("GARADOS"),
    [SPECIES_LAPRAS] = _("LAPRAS"),
    [SPECIES_DITTO] = _("DITTO"),
    [SPECIES_EEVEE] = _("EVOLI"),
    [SPECIES_VAPOREON] = _("AQUANA"),
    [SPECIES_JOLTEON] = _("BLITZA"),
    [SPECIES_FLAREON] = _("FLAMARA"),
    [SPECIES_PORYGON] = _("PORYGON"),
    [SPECIES_OMANYTE] = _("AMONITAS"),
    [SPECIES_OMASTAR] = _("AMOROSO"),
    [SPECIES_KABUTO] = _("KABUTO"),
    [SPECIES_KABUTOPS] = _("KABUTOPS"),
    [SPECIES_AERODACTYL] = _("AERODACTYL"),
    [SPECIES_SNORLAX] = _("RELAXO"),
    [SPECIES_ARTICUNO] = _("ARKTOS"),
    [SPECIES_ZAPDOS] = _("ZAPDOS"),
    [SPECIES_MOLTRES] = _("LAVADOS"),
    [SPECIES_DRATINI] = _("DRATINI"),
    [SPECIES_DRAGONAIR] = _("DRAGONIR"),
    [SPECIES_DRAGONITE] = _("DRAGORAN"),
    [SPECIES_MEWTWO] = _("MEWTU"),
    [SPECIES_MEW] = _("MEW"),
    [SPECIES_CHIKORITA] = _("ENDIVIE"),
    [SPECIES_BAYLEEF] = _("LORBLATT"),
    [SPECIES_MEGANIUM] = _("MEGANIE"),
    [SPECIES_CYNDAQUIL] = _("FEURIGEL"),
    [SPECIES_QUILAVA] = _("IGELAVAR"),
    [SPECIES_TYPHLOSION] = _("TORNUPTO"),
    [SPECIES_TOTODILE] = _("KARNIMANI"),
    [SPECIES_CROCONAW] = _("TYRACROC"),
    [SPECIES_FERALIGATR] = _("IMPERGATOR"),
    [SPECIES_SENTRET] = _("WIESOR"),
    [SPECIES_FURRET] = _("WIESENIOR"),
    [SPECIES_HOOTHOOT] = _("HOOTHOOT"),
    [SPECIES_NOCTOWL] = _("NOCTUH"),
    [SPECIES_LEDYBA] = _("LEDYBA"),
    [SPECIES_LEDIAN] = _("LEDIAN"),
    [SPECIES_SPINARAK] = _("WEBARAK"),
    [SPECIES_ARIADOS] = _("ARIADOS"),
    [SPECIES_CROBAT] = _("IKSBAT"),
    [SPECIES_CHINCHOU] = _("LAMPI"),
    [SPECIES_LANTURN] = _("LANTURN"),
    [SPECIES_PICHU] = _("PICHU"),
    [SPECIES_CLEFFA] = _("PII"),
    [SPECIES_IGGLYBUFF] = _("FLUFFELUFF"),
    [SPECIES_TOGEPI] = _("TOGEPI"),
    [SPECIES_TOGETIC] = _("TOGETIC"),
    [SPECIES_NATU] = _("NATU"),
    [SPECIES_XATU] = _("XATU"),
    [SPECIES_MAREEP] = _("VOLTILAMM"),
    [SPECIES_FLAAFFY] = _("WAATY"),
    [SPECIES_AMPHAROS] = _("AMPHAROS"),
    [SPECIES_BELLOSSOM] = _("BLUBELLA"),
    [SPECIES_MARILL] = _("MARILL"),
    [SPECIES_AZUMARILL] = _("AZUMARILL"),
    [SPECIES_SUDOWOODO] = _("MOGELBAUM"),
    [SPECIES_POLITOED] = _("QUAXO"),
    [SPECIES_HOPPIP] = _("HOPPSPROSS"),
    [SPECIES_SKIPLOOM] = _("HUBELUPF"),
    [SPECIES_JUMPLUFF] = _("PAPUNGHA"),
    [SPECIES_AIPOM] = _("GRIFFEL"),
    [SPECIES_SUNKERN] = _("SONNKERN"),
    [SPECIES_SUNFLORA] = _("SONNFLORA"),
    [SPECIES_YANMA] = _("YANMA"),
    [SPECIES_WOOPER] = _("FELINO"),
    [SPECIES_QUAGSIRE] = _("MORLORD"),
    [SPECIES_ESPEON] = _("PSIANA"),
    [SPECIES_UMBREON] = _("NACHTARA"),
    [SPECIES_MURKROW] = _("KRAMURX"),
    [SPECIES_SLOWKING] = _("LASCHOKING"),
    [SPECIES_MISDREAVUS] = _("TRAUNFUGIL"),
    [SPECIES_UNOWN] = _("ICOGNITO"),
    [SPECIES_WOBBUFFET] = _("WOINGENAU"),
    [SPECIES_GIRAFARIG] = _("GIRAFARIG"),
    [SPECIES_PINECO] = _("TANNZA"),
    [SPECIES_FORRETRESS] = _("FORSTELLKA"),
    [SPECIES_DUNSPARCE] = _("DUMMISEL"),
    [SPECIES_GLIGAR] = _("SKORGLA"),
    [SPECIES_STEELIX] = _("STAHLOS"),
    [SPECIES_SNUBBULL] = _("SNUBBULL"),
    [SPECIES_GRANBULL] = _("GRANBULL"),
    [SPECIES_QWILFISH] = _("BALDORFISH"),
    [SPECIES_SCIZOR] = _("SCHEROX"),
    [SPECIES_SHUCKLE] = _("POTTROTT"),
    [SPECIES_HERACROSS] = _("SKARABORN"),
    [SPECIES_SNEASEL] = _("SNIEBEL"),
    [SPECIES_TEDDIURSA] = _("TEDDIURSA"),
    [SPECIES_URSARING] = _("URSARING"),
    [SPECIES_SLUGMA] = _("SCHNECKMAG"),
    [SPECIES_MAGCARGO] = _("MAGCARGO"),
    [SPECIES_SWINUB] = _("QUIEKEL"),
    [SPECIES_PILOSWINE] = _("KEIFEL"),
    [SPECIES_CORSOLA] = _("CORASONN"),
    [SPECIES_REMORAID] = _("REMORAID"),
    [SPECIES_OCTILLERY] = _("OCTILLERY"),
    [SPECIES_DELIBIRD] = _("BOTOGEL"),
    [SPECIES_MANTINE] = _("MANTAX"),
    [SPECIES_SKARMORY] = _("PANZAERON"),
    [SPECIES_HOUNDOUR] = _("HUNDUSTER"),
    [SPECIES_HOUNDOOM] = _("HUNDEMON"),
    [SPECIES_KINGDRA] = _("SEEDRAKING"),
    [SPECIES_PHANPY] = _("PHANPY"),
    [SPECIES_DONPHAN] = _("DONPHAN"),
    [SPECIES_PORYGON2] = _("PORYGON2"),
    [SPECIES_STANTLER] = _("DAMHIRPLEX"),
    [SPECIES_SMEARGLE] = _("FARBEAGLE"),
    [SPECIES_TYROGUE] = _("RABAUZ"),
    [SPECIES_HITMONTOP] = _("KAPOERA"),
    [SPECIES_SMOOCHUM] = _("KUSSILLA"),
    [SPECIES_ELEKID] = _("ELEKID"),
    [SPECIES_MAGBY] = _("MAGBY"),
    [SPECIES_MILTANK] = _("MILTANK"),
    [SPECIES_BLISSEY] = _("HEITEIRA"),
    [SPECIES_RAIKOU] = _("RAIKOU"),
    [SPECIES_ENTEI] = _("ENTEI"),
    [SPECIES_SUICUNE] = _("SUICUNE"),
    [SPECIES_LARVITAR] = _("LARVITAR"),
    [SPECIES_PUPITAR] = _("PUPITAR"),
    [SPECIES_TYRANITAR] = _("DESPOTAR"),
    [SPECIES_LUGIA] = _("LUGIA"),
    [SPECIES_HO_OH] = _("HO-OH"),
    [SPECIES_CELEBI] = _("CELEBI"),
    [SPECIES_OLD_UNOWN_B] = _("?"),
    [SPECIES_OLD_UNOWN_C] = _("?"),
    [SPECIES_OLD_UNOWN_D] = _("?"),
    [SPECIES_OLD_UNOWN_E] = _("?"),
    [SPECIES_OLD_UNOWN_F] = _("?"),
    [SPECIES_OLD_UNOWN_G] = _("?"),
    [SPECIES_OLD_UNOWN_H] = _("?"),
    [SPECIES_OLD_UNOWN_I] = _("?"),
    [SPECIES_OLD_UNOWN_J] = _("?"),
    [SPECIES_OLD_UNOWN_K] = _("?"),
    [SPECIES_OLD_UNOWN_L] = _("?"),
    [SPECIES_OLD_UNOWN_M] = _("?"),
    [SPECIES_OLD_UNOWN_N] = _("?"),
    [SPECIES_OLD_UNOWN_O] = _("?"),
    [SPECIES_OLD_UNOWN_P] = _("?"),
    [SPECIES_OLD_UNOWN_Q] = _("?"),
    [SPECIES_OLD_UNOWN_R] = _("?"),
    [SPECIES_OLD_UNOWN_S] = _("?"),
    [SPECIES_OLD_UNOWN_T] = _("?"),
    [SPECIES_OLD_UNOWN_U] = _("?"),
    [SPECIES_OLD_UNOWN_V] = _("?"),
    [SPECIES_OLD_UNOWN_W] = _("?"),
    [SPECIES_OLD_UNOWN_X] = _("?"),
    [SPECIES_OLD_UNOWN_Y] = _("?"),
    [SPECIES_OLD_UNOWN_Z] = _("?"),
    [SPECIES_TREECKO] = _("GECKARBOR"),
    [SPECIES_GROVYLE] = _("REPTAIN"),
    [SPECIES_SCEPTILE] = _("GEWALDRO"),
    [SPECIES_TORCHIC] = _("FLEMMLI"),
    [SPECIES_COMBUSKEN] = _("JUNGGLUT"),
    [SPECIES_BLAZIKEN] = _("LOHGOCK"),
    [SPECIES_MUDKIP] = _("HYDROPI"),
    [SPECIES_MARSHTOMP] = _("MOORABBEL"),
    [SPECIES_SWAMPERT] = _("SUMPEX"),
    [SPECIES_POOCHYENA] = _("FIFFYEN"),
    [SPECIES_MIGHTYENA] = _("MAGNAYEN"),
    [SPECIES_ZIGZAGOON] = _("ZIGZACHS"),
    [SPECIES_LINOONE] = _("GERADAKS"),
    [SPECIES_WURMPLE] = _("WAUMPEL"),
    [SPECIES_SILCOON] = _("SCHALOKO"),
    [SPECIES_BEAUTIFLY] = _("PAPINELLA"),
    [SPECIES_CASCOON] = _("PANEKON"),
    [SPECIES_DUSTOX] = _("PUDOX"),
    [SPECIES_LOTAD] = _("LOTURZEL"),
    [SPECIES_LOMBRE] = _("LOMBRERO"),
    [SPECIES_LUDICOLO] = _("KAPPALORES"),
    [SPECIES_SEEDOT] = _("SAMURZEL"),
    [SPECIES_NUZLEAF] = _("BLANAS"),
    [SPECIES_SHIFTRY] = _("TENGULIST"),
    [SPECIES_NINCADA] = _("NINCADA"),
    [SPECIES_NINJASK] = _("NINJASK"),
    [SPECIES_SHEDINJA] = _("NINJATOM"),
    [SPECIES_TAILLOW] = _("SCHWALBINI"),
    [SPECIES_SWELLOW] = _("SCHWALBOSS"),
    [SPECIES_SHROOMISH] = _("KNILZ"),
    [SPECIES_BRELOOM] = _("KAPILZ"),
    [SPECIES_SPINDA] = _("PANDIR"),
    [SPECIES_WINGULL] = _("WINGULL"),
    [SPECIES_PELIPPER] = _("PELIPPER"),
    [SPECIES_SURSKIT] = _("GEHWEIHER"),
    [SPECIES_MASQUERAIN] = _("MASKEREGEN"),
    [SPECIES_WAILMER] = _("WAILMER"),
    [SPECIES_WAILORD] = _("WAILORD"),
    [SPECIES_SKITTY] = _("ENECO"),
    [SPECIES_DELCATTY] = _("ENEKORO"),
    [SPECIES_KECLEON] = _("KECLEON"),
    [SPECIES_BALTOY] = _("PUPPANCE"),
    [SPECIES_CLAYDOL] = _("LEPUMENTAS"),
    [SPECIES_NOSEPASS] = _("NASGNET"),
    [SPECIES_TORKOAL] = _("QURTEL"),
    [SPECIES_SABLEYE] = _("ZOBIRIS"),
    [SPECIES_BARBOACH] = _("SCHMERBE"),
    [SPECIES_WHISCASH] = _("WELSAR"),
    [SPECIES_LUVDISC] = _("LIEBISKUS"),
    [SPECIES_CORPHISH] = _("KREBSCORPS"),
    [SPECIES_CRAWDAUNT] = _("KREBUTACK"),
    [SPECIES_FEEBAS] = _("BARSCHWA"),
    [SPECIES_MILOTIC] = _("MILOTIC"),
    [SPECIES_CARVANHA] = _("KANIVANHA"),
    [SPECIES_SHARPEDO] = _("TOHAIDO"),
    [SPECIES_TRAPINCH] = _("KNACKLION"),
    [SPECIES_VIBRAVA] = _("VIBRAVA"),
    [SPECIES_FLYGON] = _("LIBELLDRA"),
    [SPECIES_MAKUHITA] = _("MAKUHITA"),
    [SPECIES_HARIYAMA] = _("HARIYAMA"),
    [SPECIES_ELECTRIKE] = _("FRIZELBLIZ"),
    [SPECIES_MANECTRIC] = _("VOLTENSO"),
    [SPECIES_NUMEL] = _("CAMAUB"),
    [SPECIES_CAMERUPT] = _("CAMERUPT"),
    [SPECIES_SPHEAL] = _("SEEMOPS"),
    [SPECIES_SEALEO] = _("SEEJONG"),
    [SPECIES_WALREIN] = _("WALRAISA"),
    [SPECIES_CACNEA] = _("TUSKA"),
    [SPECIES_CACTURNE] = _("NOKTUSKA"),
    [SPECIES_SNORUNT] = _("SCHNEPPKE"),
    [SPECIES_GLALIE] = _("FIRNONTOR"),
    [SPECIES_LUNATONE] = _("LUNASTEIN"),
    [SPECIES_SOLROCK] = _("SONNFEL"),
    [SPECIES_AZURILL] = _("AZURILL"),
    [SPECIES_SPOINK] = _("SPOINK"),
    [SPECIES_GRUMPIG] = _("GROINK"),
    [SPECIES_PLUSLE] = _("PLUSLE"),
    [SPECIES_MINUN] = _("MINUN"),
    [SPECIES_MAWILE] = _("FLUNKIFER"),
    [SPECIES_MEDITITE] = _("MEDITIE"),
    [SPECIES_MEDICHAM] = _("MEDITALIS"),
    [SPECIES_SWABLU] = _("WABLU"),
    [SPECIES_ALTARIA] = _("ALTARIA"),
    [SPECIES_WYNAUT] = _("ISSO"),
    [SPECIES_DUSKULL] = _("ZWIRRLICHT"),
    [SPECIES_DUSCLOPS] = _("ZWIRRKLOP"),
    [SPECIES_ROSELIA] = _("ROSELIA"),
    [SPECIES_SLAKOTH] = _("BUMMELZ"),
    [SPECIES_VIGOROTH] = _("MUNTIER"),
    [SPECIES_SLAKING] = _("LETARKING"),
    [SPECIES_GULPIN] = _("SCHLUPPUCK"),
    [SPECIES_SWALOT] = _("SCHLUKWECH"),
    [SPECIES_TROPIUS] = _("TROPIUS"),
    [SPECIES_WHISMUR] = _("FLURMEL"),
    [SPECIES_LOUDRED] = _("KRAKEELO"),
    [SPECIES_EXPLOUD] = _("KRAWUMMS"),
    [SPECIES_CLAMPERL] = _("PERLU"),
    [SPECIES_HUNTAIL] = _("AALABYSS"),
    [SPECIES_GOREBYSS] = _("SAGANABYSS"),
    [SPECIES_ABSOL] = _("ABSOL"),
    [SPECIES_SHUPPET] = _("SHUPPET"),
    [SPECIES_BANETTE] = _("BANETTE"),
    [SPECIES_SEVIPER] = _("VIPITIS"),
    [SPECIES_ZANGOOSE] = _("SENGO"),
    [SPECIES_RELICANTH] = _("RELICANTH"),
    [SPECIES_ARON] = _("STOLLUNIOR"),
    [SPECIES_LAIRON] = _("STOLLRAK"),
    [SPECIES_AGGRON] = _("STOLLOSS"),
    [SPECIES_CASTFORM] = _("FORMEO"),
    [SPECIES_VOLBEAT] = _("VOLBEAT"),
    [SPECIES_ILLUMISE] = _("ILLUMISE"),
    [SPECIES_LILEEP] = _("LILIEP"),
    [SPECIES_CRADILY] = _("WIELIE"),
    [SPECIES_ANORITH] = _("ANORITH"),
    [SPECIES_ARMALDO] = _("ARMALDO"),
    [SPECIES_RALTS] = _("TRASLA"),
    [SPECIES_KIRLIA] = _("KIRLIA"),
    [SPECIES_GARDEVOIR] = _("GUARDEVOIR"),
    [SPECIES_BAGON] = _("KINDWURM"),
    [SPECIES_SHELGON] = _("DRASCHEL"),
    [SPECIES_SALAMENCE] = _("BRUTALANDA"),
    [SPECIES_BELDUM] = _("TANHEL"),
    [SPECIES_METANG] = _("METANG"),
    [SPECIES_METAGROSS] = _("METAGROSS"),
    [SPECIES_REGIROCK] = _("REGIROCK"),
    [SPECIES_REGICE] = _("REGICE"),
    [SPECIES_REGISTEEL] = _("REGISTEEL"),
    [SPECIES_KYOGRE] = _("KYOGRE"),
    [SPECIES_GROUDON] = _("GROUDON"),
    [SPECIES_RAYQUAZA] = _("RAYQUAZA"),
    [SPECIES_LATIAS] = _("LATIAS"),
    [SPECIES_LATIOS] = _("LATIOS"),
    [SPECIES_JIRACHI] = _("JIRACHI"),
    [SPECIES_DEOXYS] = _("DEOXYS"),
    [SPECIES_CHIMECHO] = _("PALIMPALIM"),
    [SPECIES_RATTATA_ALOLAN] = _("RATTFRATZ"),
    [SPECIES_RATICATE_ALOLAN] = _("RATTIKARL"),
    [SPECIES_RAICHU_ALOLAN] = _("RAICHU"),
    [SPECIES_SANDSHREW_ALOLAN] = _("SANDAN"),
    [SPECIES_SANDSLASH_ALOLAN] = _("SANDAMER"),
    [SPECIES_VULPIX_ALOLAN] = _("VULPIX"),
    [SPECIES_NINETALES_ALOLAN] = _("VULNONA"),
    [SPECIES_DIGLETT_ALOLAN] = _("DIGDA"),
    [SPECIES_DUGTRIO_ALOLAN] = _("DIGDRI"),
    [SPECIES_MEOWTH_ALOLAN] = _("MAUZI"),
    [SPECIES_PERSIAN_ALOLAN] = _("SNOBILIKAT"),
    [SPECIES_GEODUDE_ALOLAN] = _("KLEINSTEIN"),
    [SPECIES_GRAVELER_ALOLAN] = _("GEOROK"),
    [SPECIES_GOLEM_ALOLAN] = _("GEOWAZ"),
    [SPECIES_PONYTA_GALARIAN] = _("PONITA"),
    [SPECIES_RAPIDASH_GALARIAN] = _("GALLOPA"),
    [SPECIES_SLOWPOKE_GALARIAN] = _("FLEGMON"),
    [SPECIES_SLOWBRO_GALARIAN] = _("LAHMUS"),
    [SPECIES_GRIMER_ALOLAN] = _("SLEIMA"),
    [SPECIES_MUK_ALOLAN] = _("SLEIMOK"),
    [SPECIES_EXEGGUTOR_ALOLAN] = _("KOKOWEI"),
    [SPECIES_MAROWAK_ALOLAN] = _("KNOGGA"),
    [SPECIES_WEEZING_GALARIAN] = _("SMOGMOG"),
    [SPECIES_ARTICUNO_GALARIAN] = _("ARKTOS"),
    [SPECIES_ZAPDOS_GALARIAN] = _("ZAPDOS"),
    [SPECIES_MOLTRES_GALARIAN] = _("LAVADOS"),
    [SPECIES_MEWTWO_ARMORED] = _("MEWTU"),
    [SPECIES_SLOWKING_GALARIAN] _("LASCHOKING"),
    [SPECIES_LUGIA_SHADOW] = _("LUGIA"),
    [SPECIES_DEOXYS_SPEED] = _("DEOXYS"),
    [SPECIES_DEOXYS_ATTACK] = _("DEOXYS"),
    [SPECIES_DEOXYS_DEFENSE] = _("DEOXYS"),
};
