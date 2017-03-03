#include<stdio.h>
#include<string.h>

#define MAX_NAME 100

struct entity {
	const char *name;
	enum {
		NO = 0,
		YES = 1
	} is_person;
};

static const struct entity entities[] = {
	{ "kaden", YES },
	{ "aedan", NO },
	{ NULL, -1 }
};

int main()
{
	char line[MAX_NAME];
	fgets(line, MAX_NAME, stdin);

	struct entity *i = NULL;
	for(i = entities; i->name; ++i){
		if(!strcmp(i->name, line)){
			break;
		}
	}

	if(i){
		printf("%s\n", i->is_person == YES ? "yes" : "no");
	}else{
		puts("idk mank");
	}
}
