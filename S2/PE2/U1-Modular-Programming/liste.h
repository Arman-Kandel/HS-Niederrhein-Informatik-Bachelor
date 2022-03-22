// ====================================
// incomplete data type
// ( forward declaration )
// ====================================
typedef struct list list_t;

// ====================================
// interface
// ====================================
list_t *create(void);

void append(list_t *l, int val);

int getValueAt(list_t *l, int pos);

void erase(list_t *l, int val);

void toScreen(list_t *l);

char getError(list_t *l); // neu !!!
void destroy(list_t *l);
