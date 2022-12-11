#define _GNU_SOURCE
#include <alinker/dlfcn.h>
#include <alinker/hook.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <bsd/string.h>
#include <malloc.h>

static int my_pthread_mutex_init (pthread_mutex_t *__mutex, __const pthread_mutexattr_t *__mutexattr)
{
  pthread_mutex_t *realmutex = malloc(sizeof(pthread_mutex_t));
  *((int *)__mutex) = (int) realmutex;
  printf("init %x\n", __mutex);
  return pthread_mutex_init(realmutex, __mutexattr);
}

static int my_pthread_mutex_lock (pthread_mutex_t *__mutex)
{
  pthread_mutex_t *realmutex = (pthread_mutex_t *) *(int *) __mutex;
  if (realmutex == NULL)
  {
      realmutex = malloc(sizeof(pthread_mutex_t));
      *((int *)__mutex) = (int) realmutex;
      pthread_mutex_init(realmutex, NULL);     
  }
  return pthread_mutex_lock(realmutex);
}

static int my_pthread_mutex_trylock (pthread_mutex_t *__mutex)
{
  pthread_mutex_t *realmutex = (pthread_mutex_t *) *(int *) __mutex;
  if (realmutex == NULL)
  {
      realmutex = malloc(sizeof(pthread_mutex_t));
      *((int *)__mutex) = (int) realmutex;
      pthread_mutex_init(realmutex, NULL);     
  }
  return pthread_mutex_trylock(realmutex);
}


static int my_pthread_mutex_unlock (pthread_mutex_t *__mutex)
{
  pthread_mutex_t *realmutex = (pthread_mutex_t *) *(int *) __mutex;
  return pthread_mutex_unlock(realmutex);
}

static int my_pthread_mutex_destroy (pthread_mutex_t *__mutex)
{
  pthread_mutex_t *realmutex = (pthread_mutex_t *) *(int *) __mutex;
  int ret = 0;
  ret = pthread_mutex_destroy(realmutex);
  free(realmutex);
  return ret;
}                               

static int my_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
  pthread_cond_t *realcond = malloc(sizeof(pthread_cond_t));
  *((int *) cond) = (int) realcond;
  return pthread_cond_init(realcond, attr);    
}

static int my_pthread_cond_destroy (pthread_cond_t *cond)
{
  pthread_cond_t *realcond = (pthread_cond_t *) *(int *) cond;
  int ret = 0;
  ret = pthread_cond_destroy(realcond);
  free(realcond);
  return ret;
}                               


static int my_pthread_cond_broadcast(pthread_cond_t *cond)
{
  pthread_cond_t *realcond = (pthread_cond_t *) *(int *) cond;
  return pthread_cond_broadcast(realcond);    
}

static int my_pthread_cond_signal(pthread_cond_t *cond)
{
  pthread_cond_t *realcond = (pthread_cond_t *) *(int *) cond;
  return pthread_cond_signal(realcond);    
}

static int my_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  pthread_cond_t *realcond = (pthread_cond_t *) *(int *) cond;

  pthread_mutex_t *realmutex = (pthread_mutex_t *) *(int *) mutex;
  if (realmutex == NULL)
  {
      realmutex = malloc(sizeof(pthread_mutex_t));
      *((int *)mutex) = (int) realmutex;
      pthread_mutex_init(realmutex, NULL);     
  }
  
  return pthread_cond_wait(realcond, realmutex);    
}

static int my_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
  pthread_cond_t *realcond = (pthread_cond_t *) *(int *) cond;

  pthread_mutex_t *realmutex = (pthread_mutex_t *) *(int *) mutex;
  if (realmutex == NULL)
  {
      realmutex = malloc(sizeof(pthread_mutex_t));
      *((int *)mutex) = (int) realmutex;
      pthread_mutex_init(realmutex, NULL);     
  }
  
  return pthread_cond_timedwait(realcond, realmutex, abstime);    
}

void hook() {
hook_hookup_symbol("pthread_create", pthread_create);
hook_hookup_symbol("pthread_exit", pthread_exit);
hook_hookup_symbol("pthread_join", pthread_join);
hook_hookup_symbol("pthread_detach", pthread_detach);
hook_hookup_symbol("pthread_self", pthread_self);
hook_hookup_symbol("pthread_equal", pthread_equal);
hook_hookup_symbol("pthread_mutex_init", my_pthread_mutex_init );
hook_hookup_symbol("pthread_mutex_lock", my_pthread_mutex_lock );
hook_hookup_symbol("pthread_mutex_unlock", my_pthread_mutex_unlock );
hook_hookup_symbol("pthread_mutex_destroy", my_pthread_mutex_destroy );
hook_hookup_symbol("pthread_once", pthread_once);
hook_hookup_symbol("pthread_mutexattr_init", pthread_mutexattr_init);
hook_hookup_symbol("pthread_mutexattr_settype", pthread_mutexattr_settype);
hook_hookup_symbol("pthread_mutex_trylock", my_pthread_mutex_trylock);   
hook_hookup_symbol("pthread_key_create", pthread_key_create);
hook_hookup_symbol("pthread_setspecific", pthread_setspecific);
hook_hookup_symbol("pthread_getspecific", pthread_getspecific);
hook_hookup_symbol("pthread_cond_init", my_pthread_cond_init);
hook_hookup_symbol("pthread_cond_broadcast", my_pthread_cond_broadcast);
hook_hookup_symbol("pthread_cond_destroy", my_pthread_cond_destroy);
hook_hookup_symbol("pthread_cond_signal", my_pthread_cond_signal);
hook_hookup_symbol("pthread_cond_wait", my_pthread_cond_wait);
hook_hookup_symbol("pthread_cond_timedwait", my_pthread_cond_timedwait);

hook_hookup_symbol("printf", printf);
hook_hookup_symbol("malloc", malloc);
hook_hookup_symbol("free", free);
hook_hookup_symbol("calloc", calloc);
hook_hookup_symbol("realloc", realloc);
hook_hookup_symbol("memalign", memalign);
hook_hookup_symbol("valloc", valloc);
hook_hookup_symbol("pvalloc", pvalloc);


hook_hookup_symbol("memccpy",memccpy); 
hook_hookup_symbol("memchr",memchr); 
hook_hookup_symbol("memrchr",memrchr); 
hook_hookup_symbol("memcmp",memcmp); 
hook_hookup_symbol("memcpy",memcpy); 
hook_hookup_symbol("memmove",memmove); 
hook_hookup_symbol("memset",memset); 
hook_hookup_symbol("memmem",memmem);
hook_hookup_symbol("index",index); 
hook_hookup_symbol("rindex",rindex); 
hook_hookup_symbol("strchr",strchr); 
hook_hookup_symbol("strrchr",strrchr); 
hook_hookup_symbol("strlen",strlen); 
hook_hookup_symbol("strcmp",strcmp); 
hook_hookup_symbol("strcpy",strcpy); 
hook_hookup_symbol("strcat",strcat); 
hook_hookup_symbol("strcasecmp",strcasecmp); 
hook_hookup_symbol("strncasecmp",strncasecmp); 
hook_hookup_symbol("strdup",strdup); 
hook_hookup_symbol("strstr",strstr); 
hook_hookup_symbol("strcasestr",strcasestr); 
hook_hookup_symbol("strtok",strtok); 
hook_hookup_symbol("strtok_r",strtok_r); 
hook_hookup_symbol("strerror",strerror); 
hook_hookup_symbol("strerror_r",strerror_r); 
hook_hookup_symbol("strnlen",strnlen); 
hook_hookup_symbol("strncat",strncat); 
hook_hookup_symbol("strndup",strndup); 
hook_hookup_symbol("strncmp",strncmp); 
hook_hookup_symbol("strncpy",strncpy); 
hook_hookup_symbol("strlcat",strlcat); 
hook_hookup_symbol("strlcpy",strlcpy); 
hook_hookup_symbol("strcspn",strcspn); 
hook_hookup_symbol("strpbrk",strpbrk); 
hook_hookup_symbol("strsep",strsep); 
hook_hookup_symbol("strspn",strspn); 
hook_hookup_symbol("strsignal",strsignal); 
hook_hookup_symbol("strcoll",strcoll); 
hook_hookup_symbol("strxfrm",strxfrm); 

hook_hookup_symbol("bcmp",bcmp); 
hook_hookup_symbol("bcopy",bcopy); 
hook_hookup_symbol("bzero",bzero); 
hook_hookup_symbol("ffs",ffs); 
hook_hookup_symbol("index",index); 
hook_hookup_symbol("rindex",rindex); 
hook_hookup_symbol("strcasecmp",strcasecmp); 
hook_hookup_symbol("strncasecmp",strncasecmp);
}

int main() {
    hook();
    void *test = android_dlopen("/home/alexander/alinker/build/lib/libc.so", RTLD_LAZY);
    if (test == NULL) {
        puts(android_dlerror());
    }
    puts("pl");
    ((void (*)(char *))android_dlsym(test, "puts"))("Hello");
}