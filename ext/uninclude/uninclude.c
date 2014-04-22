#include <ruby.h>

#ifndef RCLASS_EXT
#include <ruby/backward/classext.h>
#endif

#ifdef RUBY_2_1_x
#define RMODULE_IDENTIFIER(c) (RCLASS(c)->m_tbl_wrapper)
#else
#define RMODULE_IDENTIFIER(c) (RCLASS(c)->m_tbl)
#endif

#ifdef RUBY_2_1_x
#define UNINCLUDE_CLEAR_METHOD_CACHE(c) (rb_clear_method_cache_by_class(c))
#else
#define UNINCLUDE_CLEAR_METHOD_CACHE(c) (rb_clear_cache_by_class(c))
#endif

static void uninclude(VALUE klass, VALUE mod) {
  Check_Type(mod, T_MODULE);

  VALUE superklass = RCLASS_SUPER(klass);
  for(; superklass; klass = superklass, superklass = RCLASS_SUPER(klass)) {
    if(RMODULE_IDENTIFIER(superklass) == RMODULE_IDENTIFIER(mod)) {
      RCLASS_SUPER(klass) = RCLASS_SUPER(superklass);
      UNINCLUDE_CLEAR_METHOD_CACHE(klass);
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
