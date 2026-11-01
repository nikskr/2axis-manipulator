# Настройка SFML в Visual Studio (x64)

### 1. Подготовка файлов
*  Скачайте архив SFML (x64) с сайта sfml-dev.org и распакуйте в `C:\SFML`.
*  Скопируйте файлы `.dll` из `C:\SFML\bin` в папки:
  1. В корень проекта — для запуска проекта.
  2. В папки сборки `x64/Debug` и `x64/Release` — для прямого запуска файлов `.exe`.

### 2. Общие пути (Конфигурация: All Configurations | Платформа: x64)
* **Настройка:** Откройте свойства проекта (`Alt + Enter`).
* **Заголовки:** В `C/C++` -> `General` -> `Additional Include Directories` вставьте:
  ```text
  C:\SFML\include
  ```
* **Библиотеки:** В `Linker` -> `General` -> `Additional Library Directories` вставьте:
  ```text
  C:\SFML\lib
  ```

### 3. Конфигурация: Debug | Платформа: x64
* **Настройка:** В `Linker` -> `Input` -> `Additional Dependencies` нажмите `Edit`.
* Вставьте имена файлов с `-d`:
  ```text
  sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;
  ```

### 4. Конфигурация: Release | Платформа: x64
* **Настройка:** В `Linker` -> `Input` -> `Additional Dependencies` нажмите `Edit`.
* Вставьте имена файлов без `-d`:
  ```text
  sfml-graphics.lib;sfml-window.lib;sfml-system.lib;
  ```
