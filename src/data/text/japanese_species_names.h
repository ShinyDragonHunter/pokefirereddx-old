const u8 gJapaneseSpeciesNames[SPECIES_COUNT][JAPANESE_NAME_LENGTH + 1] = {
    [SPECIES_NONE] = _("？？？？？"),
    [SPECIES_BULBASAUR] = _("フシギダネ"),
    [SPECIES_IVYSAUR] = _("フシギソウ"),
    [SPECIES_VENUSAUR] = _("フシギバナ"),
    [SPECIES_CHARMANDER] = _("ヒトカゲ"),
    [SPECIES_CHARMELEON] = _("リザード"),
    [SPECIES_CHARIZARD] = _("リザードン"),
    [SPECIES_SQUIRTLE] = _("ゼニガメ"),
    [SPECIES_WARTORTLE] = _("カメール"),
    [SPECIES_BLASTOISE] = _("カメックス"),
    [SPECIES_CATERPIE] = _("キャタピー"),
    [SPECIES_METAPOD] = _("トランセル"),
    [SPECIES_BUTTERFREE] = _("バタフリー"),
    [SPECIES_WEEDLE] = _("ビードル"),
    [SPECIES_KAKUNA] = _("コクーン"),
    [SPECIES_BEEDRILL] = _("スピアー"),
    [SPECIES_PIDGEY] = _("ポッポ"),
    [SPECIES_PIDGEOTTO] = _("ピジョン"),
    [SPECIES_PIDGEOT] = _("ピジョット"),
    [SPECIES_RATTATA] = _("コラッタ"),
    [SPECIES_RATICATE] = _("ラッタ"),
    [SPECIES_SPEAROW] = _("オニスズメ"),
    [SPECIES_FEAROW] = _("オニドリル"),
    [SPECIES_EKANS] = _("アーボ"),
    [SPECIES_ARBOK] = _("アーボック"),
    [SPECIES_PIKACHU] = _("ピカチュウ"),
    [SPECIES_RAICHU] = _("ライチュウ"),
    [SPECIES_SANDSHREW] = _("サンド"),
    [SPECIES_SANDSLASH] = _("サンドパン"),
    [SPECIES_NIDORAN_F] = _("ニドラン♀"),
    [SPECIES_NIDORINA] = _("ニドリーナ"),
    [SPECIES_NIDOQUEEN] = _("ニドクイン"),
    [SPECIES_NIDORAN_M] = _("ニドラン♂"),
    [SPECIES_NIDORINO] = _("ニドリーノ"),
    [SPECIES_NIDOKING] = _("ニドキング"),
    [SPECIES_CLEFAIRY] = _("ピッピ"),
    [SPECIES_CLEFABLE] = _("ピクシー"),
    [SPECIES_VULPIX] = _("ロコン"),
    [SPECIES_NINETALES] = _("キュウコン"),
    [SPECIES_JIGGLYPUFF] = _("プリン"),
    [SPECIES_WIGGLYTUFF] = _("プクリン"),
    [SPECIES_ZUBAT] = _("ズバット"),
    [SPECIES_GOLBAT] = _("ゴルバット"),
    [SPECIES_ODDISH] = _("ナゾノクサ"),
    [SPECIES_GLOOM] = _("クサイハナ"),
    [SPECIES_VILEPLUME] = _("ラフレシア"),
    [SPECIES_PARAS] = _("パラス"),
    [SPECIES_PARASECT] = _("パラセクト"),
    [SPECIES_VENONAT] = _("コンパン"),
    [SPECIES_VENOMOTH] = _("モルフォン"),
    [SPECIES_DIGLETT] = _("ディグダ"),
    [SPECIES_DUGTRIO] = _("ダグトリオ"),
    [SPECIES_MEOWTH] = _("ニャース"),
    [SPECIES_PERSIAN] = _("ペルシアン"),
    [SPECIES_PSYDUCK] = _("コダック"),
    [SPECIES_GOLDUCK] = _("ゴルダック"),
    [SPECIES_MANKEY] = _("マンキー"),
    [SPECIES_PRIMEAPE] = _("オコリザル"),
    [SPECIES_GROWLITHE] = _("ガーディ"),
    [SPECIES_ARCANINE] = _("ウインディ"),
    [SPECIES_POLIWAG] = _("ニョロモ"),
    [SPECIES_POLIWHIRL] = _("ニョロゾ"),
    [SPECIES_POLIWRATH] = _("ニョロボン"),
    [SPECIES_ABRA] = _("ケーシィ"),
    [SPECIES_KADABRA] = _("ユンゲラー"),
    [SPECIES_ALAKAZAM] = _("フーディン"),
    [SPECIES_MACHOP] = _("ワンリキー"),
    [SPECIES_MACHOKE] = _("ゴーリキー"),
    [SPECIES_MACHAMP] = _("カイリキー"),
    [SPECIES_BELLSPROUT] = _("マダツボミ"),
    [SPECIES_WEEPINBELL] = _("ウツドン"),
    [SPECIES_VICTREEBEL] = _("ウツボット"),
    [SPECIES_TENTACOOL] = _("メノクラゲ"),
    [SPECIES_TENTACRUEL] = _("ドククラゲ"),
    [SPECIES_GEODUDE] = _("イシツブテ"),
    [SPECIES_GRAVELER] = _("ゴローン"),
    [SPECIES_GOLEM] = _("ゴローニャ"),
    [SPECIES_PONYTA] = _("ポニータ"),
    [SPECIES_RAPIDASH] = _("ギャロップ"),
    [SPECIES_SLOWPOKE] = _("ヤドン"),
    [SPECIES_SLOWBRO] = _("ヤドラン"),
    [SPECIES_MAGNEMITE] = _("コイル"),
    [SPECIES_MAGNETON] = _("レアコイル"),
    [SPECIES_FARFETCHD] = _("カモネギ"),
    [SPECIES_DODUO] = _("ドードー"),
    [SPECIES_DODRIO] = _("ドードリオ"),
    [SPECIES_SEEL] = _("パウワウ"),
    [SPECIES_DEWGONG] = _("ジュゴン"),
    [SPECIES_GRIMER] = _("ベトベター"),
    [SPECIES_MUK] = _("ベトベトン"),
    [SPECIES_SHELLDER] = _("シェルダー"),
    [SPECIES_CLOYSTER] = _("パルシェン"),
    [SPECIES_GASTLY] = _("ゴース"),
    [SPECIES_HAUNTER] = _("ゴースト"),
    [SPECIES_GENGAR] = _("ゲンガー"),
    [SPECIES_ONIX] = _("イワーク"),
    [SPECIES_DROWZEE] = _("スリープ"),
    [SPECIES_HYPNO] = _("スリーパー"),
    [SPECIES_KRABBY] = _("クラブ"),
    [SPECIES_KINGLER] = _("キングラー"),
    [SPECIES_VOLTORB] = _("ビリリダマ"),
    [SPECIES_ELECTRODE] = _("マルマイン"),
    [SPECIES_EXEGGCUTE] = _("タマタマ"),
    [SPECIES_EXEGGUTOR] = _("ナッシー"),
    [SPECIES_CUBONE] = _("カラカラ"),
    [SPECIES_MAROWAK] = _("ガラガラ"),
    [SPECIES_HITMONLEE] = _("サワムラー"),
    [SPECIES_HITMONCHAN] = _("エビワラー"),
    [SPECIES_LICKITUNG] = _("ベロリンガ"),
    [SPECIES_KOFFING] = _("ドガース"),
    [SPECIES_WEEZING] = _("マタドガス"),
    [SPECIES_RHYHORN] = _("サイホーン"),
    [SPECIES_RHYDON] = _("サイドン"),
    [SPECIES_CHANSEY] = _("ラッキー"),
    [SPECIES_TANGELA] = _("モンジャラ"),
    [SPECIES_KANGASKHAN] = _("ガルーラ"),
    [SPECIES_HORSEA] = _("タッツー"),
    [SPECIES_SEADRA] = _("シードラ"),
    [SPECIES_GOLDEEN] = _("トサキント"),
    [SPECIES_SEAKING] = _("アズマオウ"),
    [SPECIES_STARYU] = _("ヒトデマン"),
    [SPECIES_STARMIE] = _("スターミー"),
    [SPECIES_MR_MIME] = _("バリヤード"),
    [SPECIES_SCYTHER] = _("ストライク"),
    [SPECIES_JYNX] = _("ルージュラ"),
    [SPECIES_ELECTABUZZ] = _("エレブー"),
    [SPECIES_MAGMAR] = _("ブーバー"),
    [SPECIES_PINSIR] = _("カイロス"),
    [SPECIES_TAUROS] = _("ケンタロス"),
    [SPECIES_MAGIKARP] = _("コイキング"),
    [SPECIES_GYARADOS] = _("ギャラドス"),
    [SPECIES_LAPRAS] = _("ラプラス"),
    [SPECIES_DITTO] = _("メタモン"),
    [SPECIES_EEVEE] = _("イーブイ"),
    [SPECIES_VAPOREON] = _("シャワーズ"),
    [SPECIES_JOLTEON] = _("サンダース"),
    [SPECIES_FLAREON] = _("ブースター"),
    [SPECIES_PORYGON] = _("ポリゴン"),
    [SPECIES_OMANYTE] = _("オムナイト"),
    [SPECIES_OMASTAR] = _("オムスター"),
    [SPECIES_KABUTO] = _("カブト"),
    [SPECIES_KABUTOPS] = _("カブトプス"),
    [SPECIES_AERODACTYL] = _("プテラ"),
    [SPECIES_SNORLAX] = _("カビゴン"),
    [SPECIES_ARTICUNO] = _("フリーザー"),
    [SPECIES_ZAPDOS] = _("サンダー"),
    [SPECIES_MOLTRES] = _("ファイヤー"),
    [SPECIES_DRATINI] = _("ミニリュウ"),
    [SPECIES_DRAGONAIR] = _("ハクリュー"),
    [SPECIES_DRAGONITE] = _("カイリュー"),
    [SPECIES_MEWTWO] = _("ミュウツー"),
    [SPECIES_MEW] = _("ミュウ"),
    [SPECIES_CHIKORITA] = _("チコリータ"),
    [SPECIES_BAYLEEF] = _("ベイリーフ"),
    [SPECIES_MEGANIUM] = _("メガニウム"),
    [SPECIES_CYNDAQUIL] = _("ヒノアラシ"),
    [SPECIES_QUILAVA] = _("マグマラシ"),
    [SPECIES_TYPHLOSION] = _("バクフーン"),
    [SPECIES_TOTODILE] = _("ワニノコ"),
    [SPECIES_CROCONAW] = _("アリゲイツ"),
    [SPECIES_FERALIGATR] = _("オーダイル"),
    [SPECIES_SENTRET] = _("オタチ"),
    [SPECIES_FURRET] = _("オオタチ"),
    [SPECIES_HOOTHOOT] = _("ホーホー"),
    [SPECIES_NOCTOWL] = _("ヨルノズク"),
    [SPECIES_LEDYBA] = _("レディバ"),
    [SPECIES_LEDIAN] = _("レディアン"),
    [SPECIES_SPINARAK] = _("イトマル"),
    [SPECIES_ARIADOS] = _("アリアドス"),
    [SPECIES_CROBAT] = _("クロバット"),
    [SPECIES_CHINCHOU] = _("チョンチー"),
    [SPECIES_LANTURN] = _("ランターン"),
    [SPECIES_PICHU] = _("ピチュー"),
    [SPECIES_CLEFFA] = _("ピィ"),
    [SPECIES_IGGLYBUFF] = _("ププリン"),
    [SPECIES_TOGEPI] = _("トゲピー"),
    [SPECIES_TOGETIC] = _("トゲチック"),
    [SPECIES_NATU] = _("ネイティ"),
    [SPECIES_XATU] = _("ネイティオ"),
    [SPECIES_MAREEP] = _("メリープ"),
    [SPECIES_FLAAFFY] = _("モココ"),
    [SPECIES_AMPHAROS] = _("デンリュウ"),
    [SPECIES_BELLOSSOM] = _("キレイハナ"),
    [SPECIES_MARILL] = _("マリル"),
    [SPECIES_AZUMARILL] = _("マリルリ"),
    [SPECIES_SUDOWOODO] = _("ウソッキー"),
    [SPECIES_POLITOED] = _("ニョロトノ"),
    [SPECIES_HOPPIP] = _("ハネッコ"),
    [SPECIES_SKIPLOOM] = _("ポポッコ"),
    [SPECIES_JUMPLUFF] = _("ワタッコ"),
    [SPECIES_AIPOM] = _("エイパム"),
    [SPECIES_SUNKERN] = _("ヒマナッツ"),
    [SPECIES_SUNFLORA] = _("キマワリ"),
    [SPECIES_YANMA] = _("ヤンヤンマ"),
    [SPECIES_WOOPER] = _("ウパー"),
    [SPECIES_QUAGSIRE] = _("ヌオー"),
    [SPECIES_ESPEON] = _("エーフィ"),
    [SPECIES_UMBREON] = _("ブラッキー"),
    [SPECIES_MURKROW] = _("ヤミカラス"),
    [SPECIES_SLOWKING] = _("ヤドキング"),
    [SPECIES_MISDREAVUS] = _("ムウマ"),
    [SPECIES_UNOWN] = _("アンノーン"),
    [SPECIES_WOBBUFFET] = _("ソーナンス"),
    [SPECIES_GIRAFARIG] = _("キリンリキ"),
    [SPECIES_PINECO] = _("クヌギダマ"),
    [SPECIES_FORRETRESS] = _("フォレトス"),
    [SPECIES_DUNSPARCE] = _("ノコッチ"),
    [SPECIES_GLIGAR] = _("グライガー"),
    [SPECIES_STEELIX] = _("ハガネール"),
    [SPECIES_SNUBBULL] = _("ブルー"),
    [SPECIES_GRANBULL] = _("グランブル"),
    [SPECIES_QWILFISH] = _("ハリーセン"),
    [SPECIES_SCIZOR] = _("ハッサム"),
    [SPECIES_SHUCKLE] = _("ツボツボ"),
    [SPECIES_HERACROSS] = _("ヘラクロス"),
    [SPECIES_SNEASEL] = _("ニューラ"),
    [SPECIES_TEDDIURSA] = _("ヒメグマ"),
    [SPECIES_URSARING] = _("リングマ"),
    [SPECIES_SLUGMA] = _("マグマッグ"),
    [SPECIES_MAGCARGO] = _("マグカルゴ"),
    [SPECIES_SWINUB] = _("ウリムー"),
    [SPECIES_PILOSWINE] = _("イノムー"),
    [SPECIES_CORSOLA] = _("サニーゴ"),
    [SPECIES_REMORAID] = _("テッポウオ"),
    [SPECIES_OCTILLERY] = _("オクタン"),
    [SPECIES_DELIBIRD] = _("デリバード"),
    [SPECIES_MANTINE] = _("マンタイン"),
    [SPECIES_SKARMORY] = _("エアームド"),
    [SPECIES_HOUNDOUR] = _("デルビル"),
    [SPECIES_HOUNDOOM] = _("ヘルガー"),
    [SPECIES_KINGDRA] = _("キングドラ"),
    [SPECIES_PHANPY] = _("ゴマゾウ"),
    [SPECIES_DONPHAN] = _("ドンファン"),
    [SPECIES_PORYGON2] = _("ポリゴン2"),
    [SPECIES_STANTLER] = _("オドシシ"),
    [SPECIES_SMEARGLE] = _("ドーブル"),
    [SPECIES_TYROGUE] = _("バルキー"),
    [SPECIES_HITMONTOP] = _("カポエラー"),
    [SPECIES_SMOOCHUM] = _("ムチュール"),
    [SPECIES_ELEKID] = _("エレキッド"),
    [SPECIES_MAGBY] = _("ブビィ"),
    [SPECIES_MILTANK] = _("ミルタンク"),
    [SPECIES_BLISSEY] = _("ハピナス"),
    [SPECIES_RAIKOU] = _("ライコウ"),
    [SPECIES_ENTEI] = _("エンテイ"),
    [SPECIES_SUICUNE] = _("スイクン"),
    [SPECIES_LARVITAR] = _("ヨーギラス"),
    [SPECIES_PUPITAR] = _("サナギラス"),
    [SPECIES_TYRANITAR] = _("バンギラス"),
    [SPECIES_LUGIA] = _("ルギア"),
    [SPECIES_HO_OH] = _("ホウオウ"),
    [SPECIES_CELEBI] = _("セレビィ"),
    [SPECIES_OLD_UNOWN_B] = _("？"),
    [SPECIES_OLD_UNOWN_C] = _("？"),
    [SPECIES_OLD_UNOWN_D] = _("？"),
    [SPECIES_OLD_UNOWN_E] = _("？"),
    [SPECIES_OLD_UNOWN_F] = _("？"),
    [SPECIES_OLD_UNOWN_G] = _("？"),
    [SPECIES_OLD_UNOWN_H] = _("？"),
    [SPECIES_OLD_UNOWN_I] = _("？"),
    [SPECIES_OLD_UNOWN_J] = _("？"),
    [SPECIES_OLD_UNOWN_K] = _("？"),
    [SPECIES_OLD_UNOWN_L] = _("？"),
    [SPECIES_OLD_UNOWN_M] = _("？"),
    [SPECIES_OLD_UNOWN_N] = _("？"),
    [SPECIES_OLD_UNOWN_O] = _("？"),
    [SPECIES_OLD_UNOWN_P] = _("？"),
    [SPECIES_OLD_UNOWN_Q] = _("？"),
    [SPECIES_OLD_UNOWN_R] = _("？"),
    [SPECIES_OLD_UNOWN_S] = _("？"),
    [SPECIES_OLD_UNOWN_T] = _("？"),
    [SPECIES_OLD_UNOWN_U] = _("？"),
    [SPECIES_OLD_UNOWN_V] = _("？"),
    [SPECIES_OLD_UNOWN_W] = _("？"),
    [SPECIES_OLD_UNOWN_X] = _("？"),
    [SPECIES_OLD_UNOWN_Y] = _("？"),
    [SPECIES_OLD_UNOWN_Z] = _("？"),
    [SPECIES_TREECKO] = _("キモリ"),
    [SPECIES_GROVYLE] = _("ジュプトル"),
    [SPECIES_SCEPTILE] = _("ジュカイン"),
    [SPECIES_TORCHIC] = _("アチャモ"),
    [SPECIES_COMBUSKEN] = _("ワカシャモ"),
    [SPECIES_BLAZIKEN] = _("バシャーモ"),
    [SPECIES_MUDKIP] = _("ミズゴロウ"),
    [SPECIES_MARSHTOMP] = _("ヌマクロー"),
    [SPECIES_SWAMPERT] = _("ラグラージ"),
    [SPECIES_POOCHYENA] = _("ポチエナ"),
    [SPECIES_MIGHTYENA] = _("グラエナ"),
    [SPECIES_ZIGZAGOON] = _("ジグザグマ"),
    [SPECIES_LINOONE] = _("マッスグマ"),
    [SPECIES_WURMPLE] = _("ケムッソ"),
    [SPECIES_SILCOON] = _("カラサリス"),
    [SPECIES_BEAUTIFLY] = _("アゲハント"),
    [SPECIES_CASCOON] = _("マユルド"),
    [SPECIES_DUSTOX] = _("ドクケイル"),
    [SPECIES_LOTAD] = _("ハスボー"),
    [SPECIES_LOMBRE] = _("ハスブレロ"),
    [SPECIES_LUDICOLO] = _("ルンパッパ"),
    [SPECIES_SEEDOT] = _("タネボー"),
    [SPECIES_NUZLEAF] = _("コノハナ"),
    [SPECIES_SHIFTRY] = _("ダーテング"),
    [SPECIES_NINCADA] = _("ツチニン"),
    [SPECIES_NINJASK] = _("テッカニン"),
    [SPECIES_SHEDINJA] = _("ヌケニン"),
    [SPECIES_TAILLOW] = _("スバメ"),
    [SPECIES_SWELLOW] = _("オオスバメ"),
    [SPECIES_SHROOMISH] = _("キノココ"),
    [SPECIES_BRELOOM] = _("キノガッサ"),
    [SPECIES_SPINDA] = _("パッチール"),
    [SPECIES_WINGULL] = _("キャモメ"),
    [SPECIES_PELIPPER] = _("ペリッパー"),
    [SPECIES_SURSKIT] = _("アメタマ"),
    [SPECIES_MASQUERAIN] = _("アメモース"),
    [SPECIES_WAILMER] = _("ホエルコ"),
    [SPECIES_WAILORD] = _("ホエルオー"),
    [SPECIES_SKITTY] = _("エネコ"),
    [SPECIES_DELCATTY] = _("エネコロロ"),
    [SPECIES_KECLEON] = _("カクレオン"),
    [SPECIES_BALTOY] = _("ヤジロン"),
    [SPECIES_CLAYDOL] = _("ネンドール"),
    [SPECIES_NOSEPASS] = _("ノズパス"),
    [SPECIES_TORKOAL] = _("コータス"),
    [SPECIES_SABLEYE] = _("ヤミラミ"),
    [SPECIES_BARBOACH] = _("ドジョッチ"),
    [SPECIES_WHISCASH] = _("ナマズン"),
    [SPECIES_LUVDISC] = _("ラブカス"),
    [SPECIES_CORPHISH] = _("ヘイガニ"),
    [SPECIES_CRAWDAUNT] = _("シザリガー"),
    [SPECIES_FEEBAS] = _("ヒンバス"),
    [SPECIES_MILOTIC] = _("ミロカロス"),
    [SPECIES_CARVANHA] = _("キバニア"),
    [SPECIES_SHARPEDO] = _("サメハダー"),
    [SPECIES_TRAPINCH] = _("ナックラー"),
    [SPECIES_VIBRAVA] = _("ビブラーバ"),
    [SPECIES_FLYGON] = _("フライゴン"),
    [SPECIES_MAKUHITA] = _("マクノシタ"),
    [SPECIES_HARIYAMA] = _("ハリテヤマ"),
    [SPECIES_ELECTRIKE] = _("ラクライ"),
    [SPECIES_MANECTRIC] = _("ライボルト"),
    [SPECIES_NUMEL] = _("ドンメル"),
    [SPECIES_CAMERUPT] = _("バクーダ"),
    [SPECIES_SPHEAL] = _("タマザラシ"),
    [SPECIES_SEALEO] = _("トドグラー"),
    [SPECIES_WALREIN] = _("トドゼルガ"),
    [SPECIES_CACNEA] = _("サボネア"),
    [SPECIES_CACTURNE] = _("ノクタス"),
    [SPECIES_SNORUNT] = _("ユキワラシ"),
    [SPECIES_GLALIE] = _("オニゴーリ"),
    [SPECIES_LUNATONE] = _("ルナトーン"),
    [SPECIES_SOLROCK] = _("ソルロック"),
    [SPECIES_AZURILL] = _("ルリリ"),
    [SPECIES_SPOINK] = _("バネブー"),
    [SPECIES_GRUMPIG] = _("ブーピッグ"),
    [SPECIES_PLUSLE] = _("プラスル"),
    [SPECIES_MINUN] = _("マイナン"),
    [SPECIES_MAWILE] = _("クチート"),
    [SPECIES_MEDITITE] = _("アサナン"),
    [SPECIES_MEDICHAM] = _("チャーレム"),
    [SPECIES_SWABLU] = _("チルット"),
    [SPECIES_ALTARIA] = _("チルタリス"),
    [SPECIES_WYNAUT] = _("ソーナノ"),
    [SPECIES_DUSKULL] = _("ヨマワル"),
    [SPECIES_DUSCLOPS] = _("サマヨール"),
    [SPECIES_ROSELIA] = _("ロゼリア"),
    [SPECIES_SLAKOTH] = _("ナマケロ"),
    [SPECIES_VIGOROTH] = _("ヤルキモノ"),
    [SPECIES_SLAKING] = _("ケッキング"),
    [SPECIES_GULPIN] = _("ゴクリン"),
    [SPECIES_SWALOT] = _("マルノーム"),
    [SPECIES_TROPIUS] = _("トロピウス"),
    [SPECIES_WHISMUR] = _("ゴニョニョ"),
    [SPECIES_LOUDRED] = _("ドゴーム"),
    [SPECIES_EXPLOUD] = _("バクオング"),
    [SPECIES_CLAMPERL] = _("パールル"),
    [SPECIES_HUNTAIL] = _("ハンテール"),
    [SPECIES_GOREBYSS] = _("サクラビス"),
    [SPECIES_ABSOL] = _("アブソル"),
    [SPECIES_SHUPPET] = _("カゲボウズ"),
    [SPECIES_BANETTE] = _("ジュペッタ"),
    [SPECIES_SEVIPER] = _("ハブネーク"),
    [SPECIES_ZANGOOSE] = _("ザングース"),
    [SPECIES_RELICANTH] = _("ジーランス"),
    [SPECIES_ARON] = _("ココドラ"),
    [SPECIES_LAIRON] = _("コドラ"),
    [SPECIES_AGGRON] = _("ボスゴドラ"),
    [SPECIES_CASTFORM] = _("ポワルン"),
    [SPECIES_VOLBEAT] = _("バルビート"),
    [SPECIES_ILLUMISE] = _("イルミーゼ"),
    [SPECIES_LILEEP] = _("リリーラ"),
    [SPECIES_CRADILY] = _("ユレイドル"),
    [SPECIES_ANORITH] = _("アノプス"),
    [SPECIES_ARMALDO] = _("アーマルド"),
    [SPECIES_RALTS] = _("ラルトス"),
    [SPECIES_KIRLIA] = _("キルリア"),
    [SPECIES_GARDEVOIR] = _("サーナイト"),
    [SPECIES_BAGON] = _("タツベイ"),
    [SPECIES_SHELGON] = _("コモルー"),
    [SPECIES_SALAMENCE] = _("ボーマンダ"),
    [SPECIES_BELDUM] = _("ダンバル"),
    [SPECIES_METANG] = _("メタング"),
    [SPECIES_METAGROSS] = _("メタグロス"),
    [SPECIES_REGIROCK] = _("レジロック"),
    [SPECIES_REGICE] = _("レジアイス"),
    [SPECIES_REGISTEEL] = _("レジスチル"),
    [SPECIES_KYOGRE] = _("カイオーガ"),
    [SPECIES_GROUDON] = _("グラードン"),
    [SPECIES_RAYQUAZA] = _("レックウザ"),
    [SPECIES_LATIAS] = _("ラティアス"),
    [SPECIES_LATIOS] = _("ラティオス"),
    [SPECIES_JIRACHI] = _("ジラーチ"),
    [SPECIES_DEOXYS] = _("デオキシス"),
    [SPECIES_CHIMECHO] = _("チリーン"),
    [SPECIES_RATTATA_ALOLAN] = _("コラッタ"),
    [SPECIES_RATICATE_ALOLAN] = _("ラッタ"),
    [SPECIES_RAICHU_ALOLAN] = _("ライチュウ"),
    [SPECIES_SANDSHREW_ALOLAN] = _("サンド"),
    [SPECIES_SANDSLASH_ALOLAN] = _("サンドパン"),
    [SPECIES_VULPIX_ALOLAN] = _("ロコン"),
    [SPECIES_NINETALES_ALOLAN] = _("キュウコン"),
    [SPECIES_DIGLETT_ALOLAN] = _("ディグダ"),
    [SPECIES_DUGTRIO_ALOLAN] = _("ダグトリオ"),
    [SPECIES_MEOWTH_ALOLAN] = _("ニャース"),
    [SPECIES_PERSIAN_ALOLAN] = _("ペルシアン"),
    [SPECIES_GEODUDE_ALOLAN] = _("イシツブテ"),
    [SPECIES_GRAVELER_ALOLAN] = _("ゴローン"),
    [SPECIES_GOLEM_ALOLAN] = _("ゴローニャ"),
    [SPECIES_PONYTA_GALARIAN] = _("ポニータ"),
    [SPECIES_RAPIDASH_GALARIAN] = _("ギャロップ"),
    [SPECIES_SLOWPOKE_GALARIAN] = _("ヤドン"),
    [SPECIES_SLOWBRO_GALARIAN] = _("ヤドラン"),
    [SPECIES_GRIMER_ALOLAN] = _("ベトベター"),
    [SPECIES_MUK_ALOLAN] = _("ベトベトン"),
    [SPECIES_EXEGGUTOR_ALOLAN] = _("ナッシー"),
    [SPECIES_MAROWAK_ALOLAN] = _("ガラガラ"),
    [SPECIES_WEEZING_GALARIAN] = _("マタドガス"),
    [SPECIES_ARTICUNO_GALARIAN] = _("フリーザー"),
    [SPECIES_ZAPDOS_GALARIAN] = _("サンダー"),
    [SPECIES_MOLTRES_GALARIAN] = _("ファイヤー"),
    [SPECIES_MEWTWO_ARMORED] = _("ミュウツー"),
    [SPECIES_SLOWKING_GALARIAN] _("ヤドキング"),
    [SPECIES_LUGIA_SHADOW] = _("ルギア"),
    [SPECIES_DEOXYS_SPEED] = _("デオキシス"),
    [SPECIES_DEOXYS_ATTACK] = _("デオキシス"),
    [SPECIES_DEOXYS_DEFENSE] = _("デオキシス"),
};
