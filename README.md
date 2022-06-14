# Автоматизиран формикариум

<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Formicarium1.JPG" width="400">

Проектът представлява автоматизирана система, която е способна да удовлетвори най-базовите нужди на една мравчена колония.
Повече за целта на проекта - [тук](https://github.com/IviGeorgiev/Automated-Formicarium/blob/main/Automated_Formicarium_Documentation_Ivaylo_Georgiev.pdf)

## Какви автоматизации предлага проектът?
**Система за осветление** - Представлява панел с бели светодиоди, който е поставен върху капака на фермата. Управлява се с RTC DS1307 модул и дистанционно (Натискане на бутон №1).

**Система за напояване** - Представлява 5V помпа (DC моторче), която е свързана с тръба към гнездото. Управлява се с DHT11 модул и дистанционно (Натискане на бутон №2).

**Система за хранене** - Представлява Servo механизъм, който е поставен върху капака на фермата. Управлява се с RTC DS1307 модул и дистанционно (Натискане на бутон №3).

**LCD екран** - Показва информация за температурата и влажността в гнездото. Използва DHT11 модула.

## Изпозвани технологии
- Arduino UNO
- C++

**Библотеки:** Adafruit_BusIO; Arduino-IRremote-mater; DHTLib; RTClib
 
## Галерия
<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Formicarium3.JPG" width="500">
<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Formicarium4.JPG" width="500">

*Гнездо с камери*

<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Panel1.JPG" width="500">

*LCD дисплей и IR приемник*

<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Top3.JPG" width="500">

*Система за хранене и панел със светодиоди*

<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Watering1.JPG" width="500">

*Системата за напояване*

<img src="https://raw.githubusercontent.com/IviGeorgiev/Automated-Formicarium/main/gallery/Colony1.JPG" width="500">

Колония с царица - *Camponotus herculeanus*


## Автор
Проектът е разработен от **Ивайло Георгиев** ([Ivaylo Georgiev](https://github.com/IviGeorgiev)) - 2022

