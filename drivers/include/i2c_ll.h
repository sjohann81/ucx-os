int i2c_ll_init(struct i2c_hw_config_values_s *config_values);
int i2c_ll_deinit(struct i2c_hw_config_values_s *config_values);
int i2c_ll_start(struct i2c_hw_config_values_s *config_values);
int i2c_ll_restart(struct i2c_hw_config_values_s *config_values);
int i2c_ll_stop(struct i2c_hw_config_values_s *config_values);
int i2c_ll_wr_addr(struct i2c_hw_config_values_s *config_values, unsigned short addr);
int i2c_ll_rd_addr(struct i2c_hw_config_values_s *config_values, unsigned short addr);
int i2c_ll_write(struct i2c_hw_config_values_s *config_values, unsigned char val);
int i2c_ll_read(struct i2c_hw_config_values_s *config_values, uint8_t nack);
