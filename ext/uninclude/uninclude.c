#include <ruby.h>

#ifndef RCLASS_IV_TBL
#include <ruby/backward/classext.h>
#endif

static void uninclude(VALUE klass, VALUE mod) {
  Check_Type(mod, T_MODULE);

#ifdef RCLASS_IV_TBL
  VALUE orig = klass;
#endif
  VALUE superklass = RCLASS_SUPER(klass);
  for(; superklass; klass = superklass, superklass = RCLASS_SUPER(klass)) {
#ifdef RUBY_2_1_x
    if (RCLASS(superklass)->m_tbl_wrapper == RCLASS(mod)->m_tbl_wrapper) {
#else
    if (RCLASS(superklass)->m_tbl == RCLASS(mod)->m_tbl) {
#endif
      RCLASS_SUPER(klass) = RCLASS_SUPER(superklass);
#ifdef RUBY_2_1_x
      rb_clear_method_cache_by_class(klass);
#elif defined RCLASS_IV_TBL
      rb_clear_cache_by_class(orig);
#else
      rb_clear_cache_by_class(klass);
#endif
      break;
    }
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
