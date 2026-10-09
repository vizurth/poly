#!/usr/bin/env bash
# Пример присваивания PS1. Для изменения приглашения текущего терминала
# выполните: source ./lab3/prompt.sh

if [[ -z ${BASH_VERSION:-} ]]; then
    printf '%s\n' 'Этот пример предназначен для Bash.' >&2
    return 1 2>/dev/null || exit 1
fi

PS1='[lab3 \u@\h:\w]\$ '
export PS1
