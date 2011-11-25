#ifndef __BKS_UI_PRIVATE_H__
#define __BKS_UI_PRIVATE_H__

static Bks_Ui *ui = NULL;

void user_accounts_page_add(void);
void user_accounts_page_reset(void);

void products_page_add(void);
void products_page_reset(void);

void products_product_selected(Bks_Product *product);

#endif
