#!/usr/bin/env bash
# Демонстрация переменных окружения и настроек Bash для лабораторной работы 3.
# Изменения PATH, LANG и TERM выполняются только во вложенных subshell.

set -u

if [[ ${1:-} == "-h" || ${1:-} == "--help" ]]; then
    cat <<'HELP'
Использование: ./lab3/demo.sh [--help]

Показывает PATH, LANG, TERM и PS1 текущего процесса, затем демонстрирует
временные изменения PATH, LANG и TERM во вложенных subshell. Настройки
текущего терминала и файлы профиля не меняются.

Чтобы выбрать значение LANG для демонстрации:
  LAB3_LANG=ru_RU.UTF-8 ./lab3/demo.sh
HELP
    exit 0
fi

printf '%s\n' '== Текущие значения =='
printf 'PATH=%s\n' "${PATH-<не задан>}"
printf 'LANG=%s\n' "${LANG-<не задан>}"
printf 'TERM=%s\n' "${TERM-<не задан>}"
printf 'PS1=%s\n' "${PS1-<не задан в неинтерактивном Bash>}"

printf '\n%s\n' '== Пустой PATH (только во вложенном subshell) =='
(
    PATH=''
    export PATH
    printf 'PATH=<пусто>\n'
    if date >/dev/null 2>&1; then
        printf '%s\n' 'date доступна: возможно, она определена как функция или builtin.'
    else
        printf '%s\n' 'date не найдена, как и другие внешние команды из каталогов PATH.'
    fi
)
printf 'PATH после subshell=%s\n' "${PATH-<не задан>}"

printf '\n%s\n' '== LANG (только во вложенном subshell) =='
(
    LANG=${LAB3_LANG:-ru_RU.UTF-8}
    export LANG
    printf 'LANG=%s\n' "$LANG"
    if command -v locale >/dev/null 2>&1; then
        locale 2>&1 || printf '%s\n' 'Выбранная локаль может отсутствовать в системе.'
    else
        printf '%s\n' 'Команда locale не установлена.'
    fi
)
printf 'LANG после subshell=%s\n' "${LANG-<не задан>}"

printf '\n%s\n' '== TERM=vt100 (только во вложенном subshell) =='
(
    TERM=vt100
    export TERM
    printf 'TERM=%s\n' "$TERM"
    if command -v tput >/dev/null 2>&1; then
        printf 'Число цветов по terminfo: '
        tput colors 2>/dev/null || printf '%s\n' 'терминал не описан в terminfo'
    else
        printf '%s\n' 'Команда tput не установлена.'
    fi
)
printf 'TERM после subshell=%s\n' "${TERM-<не задан>}"

cat <<'NOTE'

Для наблюдения за поведением интерактивных программ откройте отдельный
терминал и временно выполните там:
  old_term=$TERM
  TERM=vt100; export TERM
  man bash
  # Если установлен редактор Midnight Commander:
  mcedit
Затем восстановите исходное значение TERM командой:
  export TERM=$old_term
Или закройте этот терминал.
NOTE
