#pragma once

#ifndef __GNUC__
#error "Karat uses gnu compiler extensions"
#endif

#define CONSTRUCTOR __attribute__ ((constructor))
#define DESTRUCTOR __attribute__ ((destructor))
#define PURE __attribute__ ((pure))
#define STRICT __attribute__ ((const))
#define DEPRECATED __attribute__ ((deprecated))
