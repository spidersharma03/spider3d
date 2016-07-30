#ifndef _NODEANIMATOR_H

#define _NODEANIMATOR_H

#include "Node.h"


namespace spider3d
{
  
  class STimer;

  namespace scene
  {

   
   class NodeAnimator
   {

   public:
	   
	   NodeAnimator()
	   {
		   m_Timer = 0;
	   }

	   NodeAnimator(STimer *timer)
	   {
		   m_Timer = timer;
	   }

	   void setTimer(STimer *timer)
	   {
		   m_Timer = timer;
	   }

	   virtual void animate(sf32 time) = 0;

   protected:
		
	   STimer *m_Timer;
   };

  }

}

#endif