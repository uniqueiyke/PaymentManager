#include "GwGenaralFunctions.h"
#include <QComboBox>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

QRegularExpression stringRegExp(const QString &str){
    return QRegularExpression(str, QRegularExpression::CaseInsensitiveOption);
}

QString matchString(const QString &name, const QRegularExpression &regExp){
    if(regExp.match(name).hasMatch()){
        return name;
    }

    return QString();
}

QString capitalizedEachWord(const QString &string)
{
    QString str = string;
    QStringList strList = str.split(" ");
    for(int i = 0, c = strList.count(); i < c; ++i) {
        QString s = strList.at(i);
        QString u = s.mid(0,1).toUpper();
        QString l = s.mid(1).toLower();
        QString ul = u + l;
        strList.replace(i, ul);
    }

    return strList.join(" ");
}

bool isMatched(const QString &name, const QRegularExpression &regExp)
{
    if(regExp.match(name).hasMatch()){
        return true;
    }
    return false;
}

QStringList stateLGA(const QString &state)
{
    QString abia = "Aba North,Aba South,Arochukwu,Bende,Ikwuano,Isiala Ngwa North,Isiala Ngwa South,Isuikwuato,Obi Ngwa,Ohafia,Osisioma Ngwa,Ugwunagbo,Ukwa East,Ukwa West,Umuahia North,Umuahia South,Umu Nneochi";
    QString adamawa = "Demsa,Fufore,Ganye,Girei,Gombi,Guyuk,Hong,Jada,Lamurde,Madagali,Maiha,Mayo-Belwa,Michika,Mubi North,Mubi South,Numan,Shelleng,Song,Toungo,Yola North,Yola South";
    QString anambra = "Aguata,Awka North,Awka South,Anambra East,Anambra West,Anaocha,Ayamelum,Dunukofia,Ekwusigo,Idemili North,Idemili South,Ihiala,Njikoka,Nnewi North,Nnewi South,Ogbaru,Onitsha North,Onitsha South,Orumba North,Orumba South,Oyi";
    QString akwaIbom = "Abak,Eastern Obolo,Eket,Esit-Eket,Essien Udim,Etim-Ekpo,Etinan,Ibeno,Ibesikpo-Asutan,Ibiono-Ibom,Ika,Ikono,Ikot Abasi,Ikot Ekpene,Ini,Itu,Mbo,Mkpat-Enin,Nsit-Atai,Nsit-Ibom,Nsit-Ubium,Obot-Akara,Okobo,Onna,Oron,Oruk Anam,Ukanafun,Udung-Uko,Uruan,Urue-Offong/Oruko,Uyo";
    QString bauchi = "Bauchi,Tafawa Balewa,Dass,Toro,Bogoro,Ningi,Warji,Ganjuwa,Kirfi,Alkaleri,Darazo,Misau,Giade,Shira,Jama'are,Katagum,Itas/Gadau,Zaki,Gamawa,Damban";
    QString bayelsa = "Brass,Ekeremor,Kolokuma/Opokuma,Nembe,Ogbia,Sagbama,Southern Ijaw,Yenagoa";
    QString benue = "Ado,Agatu,Apa,Buruku,Gboko,Guma,Gwer East,Gwer West,Katsina-Ala,Konshisha,Kwande,Logo,Makurdi,Obi,Ogbadibo,Ohimini,Oju,Okpokwu,Otukpo,Tarka,Ukum,Ushongo,Vandeikya";
    QString borno = "Abadan,Askira/Uba,Bama,Bayo,Biu,Chibok,Damboa,Dikwagubio,Guzamala,Gwoza,Hawul,Jere,Kaga,Kalka/Balge,Konduga,Kukawa,Kwaya-ku,Mafa,Magumeri,Maiduguri,Marte,Mobbar,Monguno,Ngala,Nganzai,Shani";
    QString crossRiver = "Abi,Akamkpa,Akpabuyo,Bakassi,Bekwarra,Biase,Boki,Calabar Municipal,Calabar South,Etung,Ikom,Obanliku,Obubra,Obudu,Odukpani,Ogoja,Yakuur,Yala";
    QString delta = "Aniocha south,Anioha,Bomadi,Burutu,Ethiope west,Ethiope east,Ika south,Ika north east,Isoko south,Isoko north,Ndokwa east,Ndokwa west,Okpe,Oshimili north,Oshimili south,Patani,Sapele,Udu,Ughelli south,Ughelli north,Ukwuani,Uviwie,Warri central,Warri north,Warri south";
    QString ebonyi = "Abakaliki,Afikpo North,Afikpo South (Edda),Ebonyi,Ezza North,Ezza South,Ikwo,Ishielu,Ivo,Izzi,Ohaozara,Ohaukwu,Onicha";
    QString enugu = "Aninri,Awgu,Enugu East,Enugu North,Enugu South,Ezeagu,Igbo Etiti,Igbo Eze North,Igbo Eze South,Isi Uzo,Nkanu East,Nkanu West,Nsukka,Oji River,Udenu,Udi,Uzo-Uwani";
    QString edo = "Akoko-Edo,Egor,Esan Central,Esan North-East,Esan South-East,Esan West,Etsako Central,Etsako East,Etsako West,Igueben,Ikpoba-Okha,Oredo,Orhionmwon,Ovia North-East,Ovia South-West,Owan East,Owan West,Uhunmwonde";
    QString ekiti = "Ado-Ekiti,Ikere,Oye,Aiyekire (Gbonyin),Efon,Ekiti East,Ekiti South-West,Ekiti West,Emure,Ido-Osi,Ijero,Ikole,Ilejemeje,Irepodun/Ifelodun,Ise/Orun,Moba";
    QString gombe = "Akko,Balanga,Billiri,Dukku,Dunakaye,Gombe,Kaltungo,Kwami,Nafada/Bajoga,Shomgom,Yamaltu/Deba";
    QString imo = "Aboh Mbaise,Ahiazu Mbaise,Ehime Mbano,Ezinihitte Mbaise,Ideato North,Ideato South,Ihitte/Uboma,Ikeduru,Isiala Mbano,Isu,Mbaitoli,Ngor Okpala,Njaba,Nkwerre,Nwangele,Obowo,Oguta,Ohaji/Egbema,Okigwe,Onuimo,Orlu,Orsu,Oru East,Oru West,Owerri Municipal,Owerri North,Owerri West,Nwangele";
    QString jigawa = "Auyo,Babura,Biriniwa,Birnin Kudu,Buji,Dutse,Gagarawa,Garki,Gumel,Guri,Gwaram,Gwiwa,Hadejia,Jahun,Kafin Hausa,Kaugama,Kazaure,Kiri Kasama,Kiyawa,Maigatari,Malam Madori,Miga,Ringim,Roni,Sule Tankarkar,Taura,Yankwashi";
    QString kaduna = "Birnin Gwari,Chikun,Giwa,Igabi,Ikara,Jaba,Jema'a,Kachia,Kaduna North,Kaduna South,Kagarko,Kajuru,Kaura,Kauru,Kubau,Kudan,Lere,Makarfi,Sabon Gari,Sanga,Soba,Zangon Kataf,Zaria";
    QString kano = "Ajingi,Albasu,Bagwai,Bebeji,Bichi,Bunkure,Dala,Dambatta,Dawakin kudu,Dawakin Tofa,Doguwa,Fagge,Gabasawa,Garko,Garun Mallam,Gaya,Gezawa,Gwale,Gwarzo,Kabo,Kano Municipal,Karaye,Kibiya,Kiru,Kumbtso,Kunchi,Kura,Madobi,Makoda,Minjibir,Nassarawa,Rano,Rimin Gado,Rogo,Shanono,Sumaila,Takai,Tarauni,Tofa,Tsanyawa,Tudun Wada,Ungogo,Warawa,Wudil";
    QString katsina = "Bakori,Batagarawa,Batsari,Baure,Bindawa,Charanchi,Dan Musa,Dandume,Danja,Daura,Dutsi,Dutsin-Ma,Faskari,Funtua,Ingawa,Jibia,Kafur,Kaita,Kankara,Kankia,Katsina,Kurfi,Kusada,Mai'Adua,Malumfashi,Mani,Mashi,Matazu,Musawa,Rimi,Sabuwa,Safana,Sandamu,Zango";
    QString kebbi = "Aleiro,Arewa Dandi,Argungu,Augie,Bagudo,Birnin Kebbi,Bunza,Dandi,Fakai,Gwandu,Jega,Kalgo,Koko/Besse,Maiyama,Ngaski,Sakaba,Shanga,Suru,Danko/Wasagu,Yauri,Zuru";
    QString kogi = "Adavi,Ajaokuta,Ankpa,Bassa,Dekina,Ibaji,Idah,Igalamela-Odolu,Ijumu,Kabba/Bunu,Koton Karfe,Lokoja,Mopa-Muro,Ofu,Ogori/Magongo,Okehi,Okene,Olamaboro,Omala,Yagba East,Yagba West";
    QString kwara = "Asa,Baruten,Edu,Ekiti,Ifelodun,Ilorin East,Ilorin South,Ilorin West,Irepodun,Isin,Kaiama,Moro,Offa,Oke Ero,Oyun,Pategi";
    QString lagos = "Agege,Alimosho Ifelodun,Alimosho,Amuwo-Odofin,Apapa,Badagry,Epe,Eti-Osa,Ibeju-Lekki,Ifako/Ijaye,Ikeja,Ikorodu,Kosofe,Lagos Island,Lagos Mainland,Mushin,Ojo,Oshodi–Isolo,Shomolu,Surulere";
    QString nasarawa = "Akwanga,Awe,Doma,Karu,Keana,Keffi,Kokona,Lafia,Nassarawa,Nassarawa/Eggon,Obi,Toto,Wamba";
    QString niger = "Agaie,Agwara,Bida,Borgu,Bosso,Chanchaga,Edati,Gbako,Gurara,Katcha,Kontagora,Lapai,Lavun,Magama,Mariga,Mashegu,Mokwa,Munya,Paikoro,Rafi,Rijau,Shiroro,Suleja,Tafa,Wushishi";
    QString ogun = "Abeokuta North,Abeokuta South,Ado-Odo/Ota,Ewekoro,Ifo,Ijebu East,Ijebu North,Ijebu North East,Ijebu Ode,Ikenne,Imeko Afon,Ipokia,Obafemi Owode,Odogbolu,Odeda,Ogun Waterside,Remo North,Sagamu,Yewa North,Yewa South";
    QString ondo = "Akoko north,Akoko north east,Akoko south east,Akoko south,Akure north,Akure,Idanre,Ifedore,Ese Odo,Ilaje,Ile Oluji/Okeigbo,Irele,Odigbo,Okitipupa,Ondo,Ondo east,Ose,Owo";
    QString osun = "Atakumosa west,Atakumosa east,Ayedaade,Ayedire,Bolawaduro,Boripe,,Ede,Ede north,Egbedore,Ejigbo,Ife north,Ife central,Ife south,Ife east,Ifedayo,Ifelodun,Ilesha west,Ila- orangun,Ilesah east,Irepodun,Irewole,Isokan,Iwo,Obokun,Odo-otin,ola oluwa,olorunda,Oriade,Orolu,Osogbo";
    QString oyo = "Akinyele Moniya,Afijio Jobele,Egbeda Egbeda,Ibadan North Agodi Gate,Ibadan North-East Iwo Road,Ibadan North-West,Ibadan South-West Ring Road,Ibadan South-East Mapo,Ibarapa Central,Ibarapa East Eruwa,Ido,Irepo,Iseyin,Kajola,Lagelu,Ogbomosho North,Ogbomosho South,Oyo West Ojongbodu,Atiba Ofa Meta,Atisbo Tede,Saki West,Saki East,Itesiwaju Otu,Iwajowa,Ibarapa North,Olorunsogo,Oluyole,Ogo Oluwa,Surulere,Orelope,Ori Ire,Oyo East,Ona Ara";
    QString plateau = "Barkin Ladi,Bassa,Bokkos,Jos East,Jos North,Jos South,Kanam,Kanke,Langtang North,Langtang South,Mangu,Mikang,Pankshin,Qua'an Pan,Riyom,Shendam,Wase";
    QString rivers = "Abua-Odual,Ahoada East,Ahoada West,Akuku-Toru,Andoni,Asari-Toru,Bonny,Degema,Eleme,Emohua,Etche,Gokana,Ikwerre,Oyigbo,Khana,Obio-Akpor,Ogba-Egbema-Ndoni,Ogu-bolo,Okrika,Omumma,Opobo-Nkoro,Portharcourt,Tai";
    QString sokoto = "Binji,Bodinga,Dange Shuni,Gada,Goronyo,Gudu,Gwadabawa,Illela,Isa,Kebbe,Kware,Rabah,Sabon Birni,Shagari,Silame,Sokoto North,Sokoto South,Tambuwal,Tangaza,Tureta,Wamako,Wurno,Yabo";
    QString taraba = "Ardo Kola,Bali,Donga,Gashaka,Gassol,Ibi,Jalingo,Karim Lamido,Kurmi,Lau,Sardauna,Takum,Ussa,Wukari,Yorro,Zing";
    QString yobe = "Bade,Bursari,Damaturu,Geidam,Gujba,Gulani,Fika,Fune,Jakusko,Karasuwa,Machina,Nangere,Nguru,Potiskum,Tarmuwa,Yunusari,Yusufari";
    QString zamfara = "Anka,Bakura,Birnin Magaji/Kiyaw,Bukkuyum,Bungudu,Tsafe,Gummi,Gusau,Kaura Namoda,Maradun,Maru,Shinkafi,Talata Mafara,Zurmi";
    QString abuja = "Abaji,Abuja Municipal,Gwagwalada,Kuje,Bwari,Kwali";

    QMap<QString, QString> stateLga;
    stateLga["Abia"] = abia;
    stateLga["Adamawa"] = adamawa;
    stateLga["Akwa Ibom"] = akwaIbom;
    stateLga["Anambra"] = anambra;
    stateLga["Bauchi"] = bauchi;
    stateLga["Bayelsa"] = bayelsa;
    stateLga["Benue"] = benue;
    stateLga["Borno"] = borno;
    stateLga["Cross River"] = crossRiver;
    stateLga["Delta"] = delta;
    stateLga["Ebonyi"] = ebonyi;
    stateLga["Enugu"] = enugu;
    stateLga["Edo"] = edo;
    stateLga["Ekiti"] = ekiti;
    stateLga["Gombe"] = gombe;
    stateLga["Imo"] = imo;
    stateLga["Jigawa"] = jigawa;
    stateLga["Kaduna"] = kaduna;
    stateLga["Kano"] = kano;
    stateLga["Katsina"] = katsina;
    stateLga["Kebbi"] = kebbi;
    stateLga["Kogi"] = kogi;
    stateLga["Kwara"] = kwara;
    stateLga["Lagos"] = lagos;
    stateLga["Nasarawa"] = nasarawa;
    stateLga["Niger"] = niger;
    stateLga["Ogun"] = ogun;
    stateLga["Ondo"] = ondo;
    stateLga["Osun"] = osun;
    stateLga["Oyo"] = oyo;
    stateLga["Plateau"] = plateau;
    stateLga["Rivers"] = rivers;
    stateLga["Sokoto"] = sokoto;
    stateLga["Taraba"] = taraba;
    stateLga["Yobe"] = yobe;
    stateLga["Zamfara"] = zamfara;
    stateLga["Abuja"] = abuja;

    QString myStateLga = stateLga[state];
    QStringList listOfLga = myStateLga.split(",", QString::SkipEmptyParts);
    return listOfLga;
}

QStringList statesInNigeria()
{
    QStringList states;
    states << "Select Your State" << "Abia" << "Adamawa" << "Akwa Ibom" << "Anambra" << "Bauchi" << "Bayelsa"
           << "Benue" << "Borno" << "Cross River" << "Delta" << "Ebonyi" << "Enugu" << "Edo" << "Ekiti" << "Gombe"
           << "Imo" << "Jigawa" << "Kaduna" << "Kano" << "Katsina" << "Kebbi" << "Kogi" << "Kwara" << "Lagos" << "Nasarawa" << "Niger"
           << "Ogun" << "Ondo" << "Osun" << "Oyo" << "Plateau" << "Rivers" << "Sokoto" << "Taraba" << "Yobe" << "Zamfara" << "Abuja";
    return states;
}

void onComboBoxTextChange(QComboBox * const comboBox, const QString &state)
{
    comboBox->clear();
    if(state == "Select Your State"){
        comboBox->addItem("Select Your Local Govt.");
    }
    comboBox->addItems(stateLGA(state));
}

QRegularExpressionValidator *validators(const QRegularExpression &re, QObject *parent)
{
    return new QRegularExpressionValidator(re, parent);
}

QString projectRootPath(const QString &rootPath)
{
    QString home = QDir::homePath();
    if(rootPath.endsWith("/")){
        home += QString("/%1%2").arg(QCoreApplication::applicationName())
                                     .arg(rootPath);
    }else {
        home += QString("/%1/%2").arg(QCoreApplication::applicationName())
                                      .arg(rootPath);
    }
    return home;
}

QString toCamelCase(const QString &str)
{
    QString copyStr = str.toLower();

    QStringList strList = copyStr.split(" ");
    int size = strList.size();
    if(size > 1){
        QString camelCase = strList.at(0);
        for(int i{1}; i < size; ++i){
            camelCase += capitalizedEachWord(strList.at(i));
        }
        return camelCase;
    }

    return copyStr;
}

QString fromCamelCase(const QString &str)
{
    QString copyStr = str;
    auto size = copyStr.size();
    auto posOfFisrtChar{0};
    QStringList strList;
    for(auto i {0}; i < size; ++i){
        if(i == size - 1){
            strList << copyStr.mid(posOfFisrtChar, i);
        }else if(copyStr.at(i).isUpper()){
            if(i == 0){
                posOfFisrtChar = i;
                continue;
            }
            strList << copyStr.mid(posOfFisrtChar, i - posOfFisrtChar);
            posOfFisrtChar = i;
        }
    }

    return strList.join(" ");
}
