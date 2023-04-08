#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "LongNumber.h"

Digit createDigit(char value) {
  Digit d = (Digit)malloc(sizeof(struct Digit)); 
  if (!d) return NULL;
  d->value = value;
  d->next = NULL;
  return d;
}

LongNumber createLongNumber() {
    LongNumber c = (LongNumber)malloc(sizeof(struct LongNumber));
    if (!c)
      return NULL;
    c->sign = 0;
    c->first = NULL;
    c->size = 0;
    c->str = NULL;
    return c;
}

void normalize(LongNumber x) {
  if (!x)
    return;
  size_t size = 0; 
  Digit d = x->first;
  Digit last = x->first;
  size_t i = 0;
  while (d) {
    ++i;
    if (size == 0) {
      ++size;
      d = d->next;
      continue;
    }
    if (d->value != 0) {
      last = d;
      size = i;
    }
    d = d->next;
  }
  d = last;
  x->size = size;
  Digit temp = d;
  d = d->next;
  temp->next = NULL;
  while (d) {
    temp = d;
    d = d->next;
    free(temp);
  }
  if (x->size == 1 && x->first->value == 0)
    x->sign = 0;
}

LongNumber longNumberCreateFromString(const char *c) {
  char sign = 0;
  size_t size = 0;

  // validation
  if (!c)
    return NULL;
  while (isspace(*c))
    ++c;
  if (*c == '+')
    ++c;
  else if (*c == '-') {
    sign = 1;
    ++c;
  }

  while (1) {
    if (isdigit(*c)) {
      ++c;
      ++size;
    }
    else {
      --c;
      break;
    }
  }
  if (size == 0)
    return NULL;
  
  LongNumber number = createLongNumber();
  if (!number)
    return NULL;
  number->sign = sign;
  number->size = size;
  Digit *p = &(number->first);
  while (isdigit(*c)) {
    *p = createDigit(*c - '0');
    if (!*p) {
      longNumberFree(number);
      return NULL;
    }
    p = &((*p)->next);
    --c;
  }
  normalize(number);
  return number;
}

LongNumber longNumberCreateFromLL(long long x) {
  LongNumber n = createLongNumber();
  if (!n)
    return NULL;
  if (x < 0)
    n->sign = 1;
  Digit *d = &(n->first);
  if (x == 0) {
    n->first = createDigit(0);
    if (!n->first) {
      longNumberFree(n);
      return NULL;
    }
    normalize(n);
    return n;
  }
  while (x) {
    *d = createDigit(abs((int)(x % 10)));
    if (!*d) {
      longNumberFree(n);
      return NULL;
    }
    d = &((*d)->next);
    x /= 10; 
  }
  normalize(n);
  return n;
}

LongNumber longNumberCopy(LongNumber x) {
  return longNumberCreateFromString(longNumberConvertToString(x));
}

const char* longNumberConvertToString(LongNumber x) {
  if (!x)
    return NULL;
  char *s = (char *)realloc(x->str, x->size + 1 + (x->sign ? 1 : 0));
  if (!s)
    return NULL;
  size_t i = x->size + (x->sign ? 1 : 0);
  s[i--] = '\0';
  Digit d = x->first;
  while (d && i >= 0) {
    s[i--] = d->value + '0';
    d = d->next;
  }
  if (x->sign)
    s[0] = '-';
  x->str = s;
  return s;
}

long long longNumberConvertToLL(LongNumber x) {
  if (!x)
    return 0;
  long long n = 0;
  long long i = 1;
  Digit d = x->first;
  while (d) {
    n += (long long)d->value * i;
    d = d->next;
    i *= 10;
  }
  if (x->sign)
    n *= -1;
  return n;
}

int longNumberGetSign(LongNumber x) {
  if (!x)
    return 100;
  if (strcmp(longNumberConvertToString(x), "0") == 0)
    return 0;
  return x->sign ? -1 : 1;
}

size_t longNumberGetDigitCount(LongNumber x) {
  if (!x)
    return -1;
  return x->size;
}

void longNumberFree(LongNumber x) {
  if (!x)
    return;
  free(x->str);
  Digit d = x->first;
  while (d) {
    Digit prev = d;
    d = d->next;
    free(prev);
  }
  free(x);
}

LongNumber longNumberAdd(LongNumber a, LongNumber b) {
  if (!a || !b)
    return NULL;
  if (a->sign == b->sign) {
    LongNumber c = createLongNumber();
    if (!c)
      return NULL;
    c->sign = a->sign;
    Digit pa = a->first, pb = b->first;
    Digit *pc = &(c->first);
    int transfer = 0;
    size_t size = 0;
    while (pa || pb || transfer) {
      int calc = transfer;
      if (pa) {
        calc += pa->value;
        pa = pa->next;
      }
      if (pb) {
        calc += pb->value;
        pb = pb->next;
      }
      *pc = createDigit(calc % 10);
      transfer = calc / 10;
      if (!*pc) {
        longNumberFree(c);
        return NULL;
      }
      pc = &((*pc)->next);
      ++size;
    }
    c->size = size;
    normalize(c);
    return c;
  }
  if (a->sign) {
    a->sign = 0;
    LongNumber c = longNumberSubtract(b, a);
    a->sign = 1;
    return c;
  }
  b->sign = 0;
  LongNumber c = longNumberSubtract(a, b);
  b->sign = 1;
  return c;
}

LongNumber longNumberSubtract(LongNumber a, LongNumber b) {
  if (!a || !b)
    return NULL;
  if (a->sign == b->sign) {
    Digit pa = a->first, pb = b->first;
    LongNumber c = createLongNumber();
    if (!c)
      return NULL;
    c->sign = a->sign;
    Digit *pc = &(c->first);
    int borrow = 0;
    while (pa) {
      int calc = pa->value;
      pa = pa->next;
      if (pb) {
        calc -= pb->value;
        pb = pb->next;
      }
      if (borrow) {
        --calc;
        borrow = 0;
      }
      if (calc < 0) {
        calc += 10;
        borrow = 1;
      }
      *pc = createDigit(calc);
      if (!*pc) {
        longNumberFree(c);
        return NULL;
      }
      pc = &((*pc)->next);
      ++c->size;
    }
    if (pb || borrow) {
      longNumberFree(c);
      c = longNumberSubtract(b, a);
      c->sign = a->sign ? 0 : 1;
      normalize(c);
      return c;
    }
    normalize(c);
    return c;
  }
  if (a->sign) {
    b->sign = 1;
    LongNumber c = longNumberAdd(a, b);
    b->sign = 0;
    return c;
  }
  b->sign = 0;
  LongNumber c = longNumberAdd(a, b);
  b->sign = 1;
  return c;
}

LongNumber longNumberMultiply(LongNumber a, LongNumber b) {
  if (!a || !b)
    return NULL;
  LongNumber res = createLongNumber();
  size_t pos = 0;
  if (!res) {
    return NULL;
  }
  Digit bd = b->first;
  while (bd) {
    LongNumber temp = createLongNumber();
    if (!temp) {
      longNumberFree(res);
      return NULL;
    }
    Digit *tempD = &(temp->first);
    for (size_t i = 0; i < pos; ++i) {
      *tempD = createDigit(0);  
      if (!*tempD) {
        longNumberFree(res);
        longNumberFree(temp);
        return NULL;
      }
      tempD = &((*tempD)->next);
      ++temp->size;
    }
    Digit ad = a->first;
    int transfer = 0;
    while (ad || transfer) {
      int calc = 0;
      calc += transfer;
      transfer = 0;
      if (ad) {
        calc += bd->value * ad->value;
        ad = ad->next;
      }
      *tempD = createDigit(calc % 10);
      if (!*tempD) {
        longNumberFree(res);
        longNumberFree(temp);
        return NULL;
      }
      transfer = calc / 10;
      tempD = &((*tempD)->next);
    }
    LongNumber temp2 = longNumberAdd(res, temp);
    if (!temp2) {
        longNumberFree(res);
        longNumberFree(temp);
        return NULL;
    }
    longNumberFree(res);
    longNumberFree(temp);
    res = temp2;
    ++pos;
    bd = bd->next;
  }
  if (a->sign + b->sign == 1)
    res->sign = 1;
  normalize(res);
  return res;
}

void longNumberDivMod(LongNumber a, LongNumber b, LongNumber *div, LongNumber *mod) {
  if (!a || !b || !div || !mod || (b->size == 1 && b->first->value == 0))
    return;
  *mod = longNumberCopy(a);
  *div = longNumberCreateFromString("0");
  LongNumber one = longNumberCreateFromString("1");
  if (!*mod || !*div || !one) {
    longNumberFree(*mod);
    longNumberFree(*div);
    longNumberFree(one);
    *div = NULL;
    *mod = NULL;
    return;
  }
  int bs = b->sign;
  (*mod)->sign = 0, b->sign = 0;
  while ((*mod)->sign == 0) {
    LongNumber temp = longNumberSubtract(*mod, b);
    if (!temp) {
      longNumberFree(*mod);
      longNumberFree(*div);
      longNumberFree(one);
      b->sign = bs;
      *div = NULL;
      *mod = NULL;
      return;
    }
    longNumberFree(*mod);
    *mod = temp;
    temp = longNumberAdd(*div, one);
    if (!temp) {
      longNumberFree(*mod);
      longNumberFree(*div);
      longNumberFree(one);
      b->sign = bs;
      *div = NULL;
      *mod = NULL;
      return;
    }
    longNumberFree(*div);
    *div = temp;
  }
  LongNumber temp = longNumberAdd(*mod, b);
  if (!temp) {
    longNumberFree(*mod);
    longNumberFree(*div);
    longNumberFree(one);
    b->sign = bs;
    *div = NULL;
    *mod = NULL;
    return;
  }
  longNumberFree(*mod);
  *mod = temp;
  temp = longNumberSubtract(*div, one);
  if (!temp) {
    longNumberFree(*mod);
    longNumberFree(*div);
    longNumberFree(one);
    b->sign = bs;
    *div = NULL;
    *mod = NULL;
    return;
  }
  longNumberFree(*div);
  *div = temp;
  b->sign = bs;
  if (a->sign)
    (*mod)->sign = 1;
  if (a->sign + b->sign == 1)
    (*div)->sign = 1;
  normalize(*div);
  normalize(*mod);
}

LongNumber longNumberDivide(LongNumber a, LongNumber b) {
  LongNumber div = NULL, mod = NULL;
  longNumberDivMod(a, b, &div, &mod);
  longNumberFree(mod);
  return div;
}

LongNumber longNumberModulo(LongNumber a, LongNumber b) {
  LongNumber div = NULL, mod = NULL;
  longNumberDivMod(a, b, &div, &mod);
  longNumberFree(div);
  return mod;
}

int longNumberCompare(LongNumber a, LongNumber b) {
  LongNumber c = longNumberSubtract(a, b);
  if (!c) {
    return 100;
  }
  char sign = c->sign;
  size_t size = c->size;
  char value = c->first->value;
  longNumberFree(c);
  if (size == 1 && value == 0)
    return 0;
  if (sign)
    return -1;
  return 1;
}
