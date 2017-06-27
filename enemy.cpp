
#include "enemy.h"

enemy::enemy(float x_in,float y_in,char dirrection,GLuint *tex,physics *world_in)
{
	m_texture=tex;
	m_dir=dirrection;
	m_health=6;
	m_x=x_in;
	m_y=y_in;
	m_sizeX=1.6;
	m_sizeY=1.1;
	m_world=world_in;
	m_deathAnim =new animation(m_texture,0.25,0,2,1,4,2,ONCE);
	m_walkAnim  =new animation(m_texture,0.25,0,0,1,4,5,LOOP);
	m_curAnim = m_walkAnim;
	m_shootAnim  =new animation(m_texture,0.25,0,1,1,4,3,ONCE);
	m_grenade.burstAnim = new animation(m_texture,0.125,0,7,1,4,2,ONCE);
	m_grenade.flyAnim = new animation(m_texture,0.25,0.125,0,6,1,1,2,ONCE);
	m_grenade.damage=1;
	m_grenade.falling=true;
	m_grenade.spriteX=-0.75;
	m_grenade.spriteY=-0.5;
	m_grenade.foe=true;
}	
	
		//
inline void enemy::self_destruct()
{
	if (m_curAnim!=m_deathAnim)
	{
		m_health=0;
		m_curAnim=m_deathAnim;
		m_speedX=0;
		m_y-=0.5;
		m_dir=-1;
	}
}

void enemy::update(player* p1)
{
	if (m_health>0){
		if (m_speedY<=0)
		{
			if (m_world->collide(m_x+0.05,m_y)||m_world->collide(m_x+m_sizeX-0.05,m_y))m_y=floor(m_y+1);
	
		}
		if(m_reload>0)m_reload--;
		if (m_shotLeft>0)
		{
			m_speedX=0;
			m_curAnim=m_shootAnim;
			if(m_heat<=0)
			{
			 	m_heat=10;
			 	m_world->addShot(m_x+m_sizeX*0.5,m_y+m_sizeY-0.4,0.4*m_dir,0.4f,&m_grenade);
			 	m_shotLeft--;
			}
			else m_heat--;
				
		}
		else
		{
			m_speedX=1;
			m_curAnim=m_walkAnim;
		}
		m_x+=0.05*m_speedX*m_dir;
		float hurt=m_world->hit(m_x,m_y,m_x+m_sizeX,m_y+m_sizeY,false);
		if(hurt>0) m_hit=true;
		m_health-=hurt;
		if (((m_dir>0)&&(m_world->collide(m_x+m_sizeX,m_y)||!m_world->collide(m_x+m_sizeX,m_y-1) ))
			||((m_dir<0)&&(m_world->collide(m_x,m_y)||!m_world->collide(m_x,m_y-1) ))) 
			m_dir=-m_dir;
		if (m_health<=0) self_destruct();
		float y1=m_y+m_sizeY*0.5;
		
		if (p1->collide(m_x+0.1,y1)||p1->collide(m_x+m_sizeX-0.1,y1)) p1->hurt(0.7f);
		if ((floor(p1->getY())==floor(m_y))&&(floor(p1->getFrontX()-m_x)==3*m_dir)&&(m_reload<=0)) 
		{
			m_reload=60;
			m_shotLeft=4;
		}
	}
}
		
		
void enemy::draw()
{
	float x1,x2;
	if (m_dir>0)
	{
		x2=m_x+m_sizeX;
		x1=x2-2;
	}
	else
	{
		x1=m_x;
		x2=x1+2;
	}
	if (m_hit) glColor3f(1,0.5,0);
	m_curAnim->draw(-m_dir,x1,m_y,x2,m_y+2);
	m_hit=false;
}

