

---



\## 6.2 Docs/PROJECT\_REPORT\_2026-01-10.md



```md

\# PROJECT REPORT — 2026-01-10



\## Контекст

Проект на UE 5.7. Используется Third Person Story Adventure Template (TPSAT), у которого предметы “используются” через UseAction:

\- UI инвентаря вызывает `UseItem`

\- далее вызывается `Use Action` (наследник `BP\_Base\_UseAction`)

\- у UseAction вызывается `RunScript`



\## Цель текущего этапа

Сделать оружие полноценным предметом инвентаря:

\- поднял → попало в инвентарь

\- “использовал” → экипировал в руку

\- можно “убрать оружие”

\- далее добавить стрельбу/удары и зомби



\## Реализовано в C++

\### ABaseWeapon

\- `UStaticMeshComponent\* WeaponMesh`

\- `FName AttachSocketName` (по умолчанию `hand\_r\_socket`)



\### UWeaponManagerComponent

\- поле `CurrentWeapon`

\- методы:

&nbsp; - `EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass)` — спавн + attach к сокету

&nbsp; - `UnequipCurrentWeapon()` — destroy + nullptr

&nbsp; - `GetCurrentWeapon()` — getter для BP



\## Интеграция с Blueprint (TPSAT UseAction)

Собирается `BP\_UseAction\_Pistol`:

\- `GetPlayerCharacter` → `Cast to BP\_StoryAdv\_Character`

\- `GetComponentByClass(WeaponManagerComponent)`

\- `IsValid` → `EquipWeapon(BP\_Pistol)`

\- (опционально) отдельный attach в BP не нужен, т.к. attach уже в C++



\## Важные заметки/грабли

\- Live Coding мешает сборке из VS: надо закрывать Editor или выключать Live Coding при полной сборке.

\- Если BP не видит новую UFUNCTION — иногда помогает:

&nbsp; - закрыть UE Editor

&nbsp; - rebuild в VS

&nbsp; - открыть UE

&nbsp; - (иногда) Refresh All Nodes в BP



\## Следующие шаги

1\) Упростить BP\_UseAction\_Pistol: оставить только EquipWeapon (без Attach в BP).

2\) Сделать “pickup” оружия как предмет инвентаря (TPSAT item data).

3\) Добавить Fire/Reload/Ammo (минимально).

4\) Скелетный зомби: AI + HP + реакции на попадания.



