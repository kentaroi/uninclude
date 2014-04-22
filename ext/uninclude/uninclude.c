#include <ruby.h>

#ifndef RCLASS_M_TBL
#define RCLASS_M_TBL(c) (RCLASS(c)->m_tbl)
#endif

#ifndef RUBY_BACKWARD_CLASSEXT_H
static VALUE rb_class_get_superclass(VALUE klass) {
    return RCLASS_SUPER(klass);
}
#endif

static void uninclude(VALUE klass, VALUE mod) {
  Check_Type(mod, T_MODULE);

  VALUE superklass = rb_class_get_superclass(klass);
  VALUE lastklass = 0;
  for(; superklass; klass = superklass, klass = rb_class_get_superclass(klass)) {
    if(lastklass == klass) break;
    if(klass == mod || RCLASS_M_TBL(superklass) == RCLASS_M_TBL(mod)) {
      rb_class_get_superclass(klass) = rb_class_get_superclass(superklass);
      rb_clear_cache_by_class(klass);
      break;
    }
    lastklass = klass;
  };
};

static VALUE rb_m_uninclude(VALUE self, VALUE mod) {
  uninclude(self, mod);
  return self;
}

static VALUE rb_m_unextend(VALUE self, VALUE mod) {
  uninclude(rb_singleton_class(self), mod);
  return self;
}

void Init_uninclude(void) {
  rb_define_private_method(rb_cModule, "uninclude", RUBY_METHOD_FUNC(rb_m_uninclude), 1);
  rb_define_method(rb_cObject, "unextend", RUBY_METHOD_FUNC(rb_m_unextend), 1);
}
