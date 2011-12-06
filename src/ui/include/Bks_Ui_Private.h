#ifndef __BKS_UI_PRIVATE_H__
#define __BKS_UI_PRIVATE_H__

static Bks_Ui *ui = NULL;

// user acounts page related functions
void user_accounts_page_add(void);
void user_accounts_page_reset(void);

// products page related functions
void products_page_add(void);
void products_page_reset(void);

void products_product_selected(Bks_Product *product);

#endif
