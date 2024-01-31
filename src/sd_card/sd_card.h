/*
 * Initialize sd card to read and write
 */
void sd_card_init();


void sd_init();

void wait();
/*
 * Returns:
 * Success: 1
 * Failure: 0
 */
int sd_card_write_data(unsigned int adress, unsigned int data);

int sd_card_read_data(unsigned int adress);