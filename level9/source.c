#include <string.h>

/* Decompiled using Ghidra */

struct N {
	void	fun();
	char	buffer[0x64];		// 0d100
	int		value;
};

void __thiscall N::N(N *this,int param_1)
{
  *(undefined ***)this = &PTR_operator+_08048848;
  *(int *)(this + 0x68) = param_1;							// 0d104
  return;
}

int __thiscall N::operator+(N *this,N *param_1)
{
  return *(int *)(param_1 + 0x68) + *(int *)(this + 0x68);	// 0d104
}

int __thiscall N::operator-(N *this,N *param_1)
{
  return *(int *)(this + 0x68) - *(int *)(param_1 + 0x68);	// 0d104
}

void __thiscall N::setAnnotation(N *this,char *param_1)
{
  size_t __n;
  
  __n = strlen(param_1);
  memcpy(this + 4,param_1,__n);
  return;
}

void main(int param_1,int param_2)
{
  N *this;
  N *this_00;
  
  if (param_1 < 2) {
                    /* WARNING: Subroutine does not return */
    _exit(1);
  }
  this = operator.new(0x6c);		// 0d108
  N::N(this,5);									/* assign 5 to this */
  this_00 = operator.new(0x6c);		// 0d108
  N::N(this_00,6);								/* assign 6 to this_00 */
  this->setAnnotation(argv[1]); /* N::setAnnotation(this,*(char **)(param_2 + 4)); */
  (this_00->*(this_00->fun))(this);										/* (*(code *)**(undefined4 **)this_00)(this_00,this); */
  return;
}

void frame_dummy(void)
{
  return;
}
