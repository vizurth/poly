(deftemplate attr
  (slot name)
  (slot val))

(deffunction ask-choice (?question ?v1 ?d1 ?v2 ?d2)
  (printout t crlf ?question crlf)
  (printout t "  1 - " ?d1 crlf)
  (printout t "  2 - " ?d2 crlf)
  (printout t "Ваш выбор (1/2): ")
  (bind ?ans (read))
  (while (and (neq ?ans 1) (neq ?ans 2)) do
    (printout t "Введите только 1 или 2: ")
    (bind ?ans (read)))
  (if (eq ?ans 1) then ?v1 else ?v2))

(defrule start
  (declare (salience 100))
  (initial-fact)
  =>
  (printout t crlf)
  (printout t "============================================================" crlf)
  (printout t " Экспертная система: выбор тактики нападения в баскетболе" crlf)
  (printout t "============================================================" crlf))

;;; ЯРУС 0
(defrule q0-nacalo-ataki
  (not (attr (name nacalo-ataki)))
  =>
  (bind ?ans (ask-choice
    "Как команда начинает атаку?"
    perehod "В переход -- сразу после перехвата/подбора"
    poziciya "Позиционно -- после расстановки"))
  (assert (attr (name nacalo-ataki) (val ?ans))))

;;; ЯРУС 1
(defrule q1-tempo-perehoda
  (attr (name nacalo-ataki) (val perehod))
  (not (attr (name tempo-perehoda)))
  =>
  (bind ?ans (ask-choice
    "Когда развивается переход?"
    pervichniy "В первые 2-3 секунды (первичный переход)"
    vtorichniy "На 4-8 секунде (вторичный переход)"))
  (assert (attr (name tempo-perehoda) (val ?ans))))

(defrule q2-tip-zachity-sopern
  (attr (name nacalo-ataki) (val poziciya))
  (not (attr (name tip-zachity-sopern)))
  =>
  (bind ?ans (ask-choice
    "Какую защиту использует соперник?"
    lichnay "Личная опека"
    zonnay "Зонная защита"))
  (assert (attr (name tip-zachity-sopern) (val ?ans))))

;;; ЯРУС 2
(defrule q3-zavershenie-bystroy-ataki
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (not (attr (name zavershenie-bystroy-ataki)))
  =>
  (bind ?ans (ask-choice
    "Как завершается быстрая атака?"
    licom "Лицом к кольцу"
    spinoy "Спиной к кольцу"))
  (assert (attr (name zavershenie-bystroy-ataki) (val ?ans))))

(defrule q4-organizaciya-vtoroy-volny
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (not (attr (name organizaciya-vtoroy-volny)))
  =>
  (bind ?ans (ask-choice
    "Как организована атака второй волны?"
    zaslon "Через заслон на мяч (пик-н-ролл / пик-н-поп)"
    bez-zaslona "Без заслона (ранний бросок/нападение)"))
  (assert (attr (name organizaciya-vtoroy-volny) (val ?ans))))

(defrule q5-preimuchestvo-vs-lichnoy
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (not (attr (name preimuchestvo-vs-lichnoy)))
  =>
  (bind ?ans (ask-choice
    "Как создается преимущество против личной защиты?"
    individualno "Индивидуальный обыгрыш (без заслона)"
    zaslon-bez-myaca "Через заслон (игрок без мяча)"))
  (assert (attr (name preimuchestvo-vs-lichnoy) (val ?ans))))

(defrule q6-ataka-zonnoy
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (not (attr (name ataka-zonnoy)))
  =>
  (bind ?ans (ask-choice
    "Как атакуется зонная защита?"
    cherez-centr "Через центрового в высокой позиции"
    cherez-perim "Через движение периметровых игроков"))
  (assert (attr (name ataka-zonnoy) (val ?ans))))

;;; ЯРУС 3
(defrule q7-sposob-polucheniya-myaca-licom
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (attr (name zavershenie-bystroy-ataki) (val licom))
  (not (attr (name sposob-polucheniya-myaca-licom)))
  =>
  (bind ?ans (ask-choice
    "Как игрок получает мяч при атаке лицом к кольцу?"
    kat "Рывок к кольцу (кат в открытое пространство)"
    odin-na-odin "1 на 1 лицом к защитнику"))
  (assert (attr (name sposob-polucheniya-myaca-licom) (val ?ans))))

(defrule q8-poziciya-priema-myaca
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (attr (name zavershenie-bystroy-ataki) (val spinoy))
  (not (attr (name poziciya-priema-myaca)))
  =>
  (bind ?ans (ask-choice
    "Игрок принимает мяч в какой позиции?"
    bystry-brosok "Быстрый бросок (после вбрасывания/потери)"
    post "Розыгрыш в посту спиной к кольцу"))
  (assert (attr (name poziciya-priema-myaca) (val ?ans))))

(defrule q9-dejstvie-igr-stavyach-zaslon
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (attr (name organizaciya-vtoroy-volny) (val zaslon))
  (not (attr (name dejstvie-igr-stavyach-zaslon)))
  =>
  (bind ?ans (ask-choice
    "Что делает игрок, ставящий заслон?"
    roll "Выход к кольцу (roll)"
    pop "Выход на периметр под бросок (pop)"))
  (assert (attr (name dejstvie-igr-stavyach-zaslon) (val ?ans))))

(defrule q10-moment-broski-posle-pasa
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (attr (name organizaciya-vtoroy-volny) (val bez-zaslona))
  (not (attr (name moment-broski-posle-pasa)))
  =>
  (bind ?ans (ask-choice
    "Когда выполняется бросок после получения мяча?"
    srazu "Сразу после паса (статичная позиция)"
    do-rasst "До завершения расстановки защиты соперника"))
  (assert (attr (name moment-broski-posle-pasa) (val ?ans))))

(defrule q11-sposob-obygrysha-bez-zaslona
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (attr (name preimuchestvo-vs-lichnoy) (val individualno))
  (not (attr (name sposob-obygrysha-bez-zaslona)))
  =>
  (bind ?ans (ask-choice
    "Как нападающий обыгрывает защитника?"
    v-perehode "Обыгрыш в переходе (без заслона, 2-я волна)"
    pozicionno "Позиционный обыгрыш без помощи партнёров"))
  (assert (attr (name sposob-obygrysha-bez-zaslona) (val ?ans))))

(defrule q12-adresat-zaslona-bez-myaca
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (attr (name preimuchestvo-vs-lichnoy) (val zaslon-bez-myaca))
  (not (attr (name adresat-zaslona-bez-myaca)))
  =>
  (bind ?ans (ask-choice
    "Кому адресован заслон без мяча?"
    ryvok "Игроку для рывка к кольцу"
    hendof "Игроку под хэнд-офф"))
  (assert (attr (name adresat-zaslona-bez-myaca) (val ?ans))))

(defrule q13-poziciya-centrovogo-vs-zona
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (attr (name ataka-zonnoy) (val cherez-centr))
  (not (attr (name poziciya-centrovogo-vs-zona)))
  =>
  (bind ?ans (ask-choice
    "Где располагается центровой против зоны?"
    vysokiy-post "На линии штрафных (высокий пост)"
    nizkiy-post "Под кольцом (низкий пост)"))
  (assert (attr (name poziciya-centrovogo-vs-zona) (val ?ans))))

(defrule q14-peredacha-cherez-zonu
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (attr (name ataka-zonnoy) (val cherez-perim))
  (not (attr (name peredacha-cherez-zonu)))
  =>
  (bind ?ans (ask-choice
    "Как передается мяч через защиту зоны?"
    cherez-verh "Через верхнего игрока (прорыв в центр зоны)"
    perimet "Через периметр (атака углов зоны)"))
  (assert (attr (name peredacha-cherez-zonu) (val ?ans))))

;;; ЯРУС 4: ЛИСТЬЯ
(defrule leaf-01
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (attr (name zavershenie-bystroy-ataki) (val licom))
  (attr (name sposob-polucheniya-myaca-licom) (val kat))
  =>
  (printout t crlf "Первичное переходное нападение (Primary transition)" crlf)
  (halt))

(defrule leaf-02
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (attr (name zavershenie-bystroy-ataki) (val licom))
  (attr (name sposob-polucheniya-myaca-licom) (val odin-na-odin))
  =>
  (printout t crlf "1 на 1 лицом к корзине (1:1 facing the basket)" crlf)
  (halt))

(defrule leaf-03
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (attr (name zavershenie-bystroy-ataki) (val spinoy))
  (attr (name poziciya-priema-myaca) (val bystry-brosok))
  =>
  (printout t crlf "Прочие нападения (бросок после вбрасывания) (Other offenses)" crlf)
  (halt))

(defrule leaf-04
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val pervichniy))
  (attr (name zavershenie-bystroy-ataki) (val spinoy))
  (attr (name poziciya-priema-myaca) (val post))
  =>
  (printout t crlf "1 на 1 спиной к корзине (1:1 back to basket)" crlf)
  (halt))

(defrule leaf-05
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (attr (name organizaciya-vtoroy-volny) (val zaslon))
  (attr (name dejstvie-igr-stavyach-zaslon) (val roll))
  =>
  (printout t crlf "Пик-н-ролл (заслон + выход к кольцу) (Pick and roll)" crlf)
  (halt))

(defrule leaf-06
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (attr (name organizaciya-vtoroy-volny) (val zaslon))
  (attr (name dejstvie-igr-stavyach-zaslon) (val pop))
  =>
  (printout t crlf "Пик-н-поп (заслон + выход на бросок) (Pick & Pop)" crlf)
  (halt))

(defrule leaf-07
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (attr (name organizaciya-vtoroy-volny) (val bez-zaslona))
  (attr (name moment-broski-posle-pasa) (val srazu))
  =>
  (printout t crlf "Спот-ап (бросок сразу после паса) (Spot-up)" crlf)
  (halt))

(defrule leaf-08
  (attr (name nacalo-ataki) (val perehod))
  (attr (name tempo-perehoda) (val vtorichniy))
  (attr (name organizaciya-vtoroy-volny) (val bez-zaslona))
  (attr (name moment-broski-posle-pasa) (val do-rasst))
  =>
  (printout t crlf "Раннее нападение (атака до расстановки защиты) (Early offense)" crlf)
  (halt))

(defrule leaf-09
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (attr (name preimuchestvo-vs-lichnoy) (val individualno))
  (attr (name sposob-obygrysha-bez-zaslona) (val v-perehode))
  =>
  (printout t crlf "Вторичное переходное нападение (Secondary transition)" crlf)
  (halt))

(defrule leaf-10
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (attr (name preimuchestvo-vs-lichnoy) (val individualno))
  (attr (name sposob-obygrysha-bez-zaslona) (val pozicionno))
  =>
  (printout t crlf "Позиционное нападение против личной защиты (Set offense on man to man)" crlf)
  (halt))

(defrule leaf-11
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (attr (name preimuchestvo-vs-lichnoy) (val zaslon-bez-myaca))
  (attr (name adresat-zaslona-bez-myaca) (val ryvok))
  =>
  (printout t crlf "Рывок / Кат (движение к/от кольца) (Cut to/from basket)" crlf)
  (halt))

(defrule leaf-12
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val lichnay))
  (attr (name preimuchestvo-vs-lichnoy) (val zaslon-bez-myaca))
  (attr (name adresat-zaslona-bez-myaca) (val hendof))
  =>
  (printout t crlf "Хэнд-офф (передача из рук в руки) (Handoff)" crlf)
  (halt))

(defrule leaf-13
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (attr (name ataka-zonnoy) (val cherez-centr))
  (attr (name poziciya-centrovogo-vs-zona) (val vysokiy-post))
  =>
  (printout t crlf "Заслон для игрока без мяча (Off-ball screen)" crlf)
  (halt))

(defrule leaf-14
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (attr (name ataka-zonnoy) (val cherez-centr))
  (attr (name poziciya-centrovogo-vs-zona) (val nizkiy-post))
  =>
  (printout t crlf "Позиционное нападение (стандартный розыгрыш) (Set offense)" crlf)
  (halt))

(defrule leaf-15
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (attr (name ataka-zonnoy) (val cherez-perim))
  (attr (name peredacha-cherez-zonu) (val cherez-verh))
  =>
  (printout t crlf "Нападение против зонной (через центра) (Set offense on the zone)" crlf)
  (halt))

(defrule leaf-16
  (attr (name nacalo-ataki) (val poziciya))
  (attr (name tip-zachity-sopern) (val zonnay))
  (attr (name ataka-zonnoy) (val cherez-perim))
  (attr (name peredacha-cherez-zonu) (val perimet))
  =>
  (printout t crlf "Нападение против зонной (через периметр) (Set offense on the zone)" crlf)
  (halt))