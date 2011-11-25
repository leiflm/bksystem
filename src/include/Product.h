typedef struct _Product Product;

struct _Product {
     unsigned long long ean;
     char *name;
     double price;
     const unsigned char *image;
};
