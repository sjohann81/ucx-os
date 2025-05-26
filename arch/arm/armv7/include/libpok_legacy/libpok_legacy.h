typedef int pok_bool_t;

/*
 * Return minimal value, which is greater-or-equal than given value
 * and has corresponded alignment.
 * 
 * @align should be constant and be power of 2.
 */
//  static inline unsigned long ALIGN_VAL(unsigned long val, unsigned long align)
//  {
//     return (val + align - 1) & (~(align - 1));
//  }