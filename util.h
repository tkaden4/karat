#pragma once

#include"log.h"

static inline wchar_t *wcsig(const wchar_t *str, wchar_t c)
{
	err_on(!str, "str not allocated");
	while(*str && *str == c){
		++str;
	}
	return (wchar_t *)str;
}

static inline wchar_t *wcsig_until(const wchar_t *str, wchar_t c)
{
	err_on(!str, "str not allocated");
	while(*str && *str != c){
		++str;
	}
	return (wchar_t *)str;
}

static inline wchar_t *wcsig_f(const wchar_t *str, int(*pred)(wint_t))
{
	err_on(!str, "str not allocated");
	while(*str && pred(*str)){
		++str;
	}
	return (wchar_t *)str;
}

static inline wchar_t *wcstrim(wchar_t *wstr, wchar_t ch)
{
	while(*wstr && *wstr == ch){
		++wstr;
	}
	size_t len = wcslen(wstr);
	while(wstr[len--] == ch){
		wstr[len] = L'\0';
	}
	return wstr;
}

static inline wchar_t *wcstrim_f(wchar_t *wstr, int(*pred)(wint_t))
{
	while(*wstr && pred(*wstr)){
		++wstr;
	}
	size_t len = wcslen(wstr);
	while(pred(wstr[len--])){
		wstr[len] = L'\0';
	}
	return wstr;
}

#define RMSPACE(wb) \
	while(*wb == L' ' || *wb == L'\t') ++wb;

static inline void rmrn(const wchar_t *str)
{
	wchar_t *n = wcsrchr(str, L'\n');
	if(n){
		*n = L'\0';
	}
}

static inline int starts_with(const wchar_t *first, const wchar_t *second)
{
	err_on(!first, "first not allocated");
	err_on(!second, "second not allocated");

	const size_t first_len = wcslen(first);
	const size_t second_len = wcslen(second);
	if(first_len >= second_len){
		return !wcsncmp(first, second, second_len);
	}
	return 0;
}
