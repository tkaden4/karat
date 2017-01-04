#pragma once

static inline void rmrn(const wchar_t *str)
{
	wchar_t *n = wcsrchr(str, L'\n');
	if(n){
		*n = L'\0';
	}
}
