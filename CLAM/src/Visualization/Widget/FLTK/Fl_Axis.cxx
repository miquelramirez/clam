#include "Fl_Axis.hxx"
#include <cmath>

namespace CLAMVM
{

const int		Fl_Axis::CANVAS_BORDER = 0;                //gap between the graphics and surrounding"box"
const int        Fl_Axis::AXIS_BORDER = 0;                  //gap between axis drawing(i.e.axis line) and its "box"
const int        Fl_Axis::MINOR_INTERVAL = 0;               //0 stands for automatic choice in default_*_intervals array
const int        Fl_Axis::MINOR_SEPARATION = 18;            
const int        Fl_Axis::MAJOR_STEP = 5;
const int        Fl_Axis::LABEL_STEP = 10;
const int        Fl_Axis::LABEL_SIZE = FL_AXIS_DEFAULT_LABEL_SIZE;
const Fl_Font    Fl_Axis::LABEL_FONT = FL_HELVETICA;


const int Fl_Axis::MAX_LABEL_FORMAT = 16;
const int Fl_Axis::MAX_LABEL_LENGTH = 32;


const int Fl_Axis::NO_LIN_DEFAULTS=3;
const double Fl_Axis::default_lin_intervals[] = {1, 2, 5};
const int Fl_Axis::default_lin_major_steps[] = {5, 5, 2};
const int Fl_Axis::default_lin_label_steps[] = {10, 5, 4};

const int Fl_Axis::NO_LOG_DEFAULTS = 3;
const double Fl_Axis::default_log_intervals[] = {1, 2, 5};
const int Fl_Axis::default_log_major_steps[] = {5, 5, 2};
const int Fl_Axis::default_log_label_steps[] = {10, 5, 2};


void Fl_Axis::minimum(double x)
{
		proxy_min = x;
		min_=x;
		if(!valid_)
		{
				max_=x;
				valid_=1;
		}
		damage(FL_AXIS_DAMAGE_ALL);
		update();

};

void Fl_Axis::maximum(double x)
{
		proxy_max = x;
		max_=x;
		if(!valid_)
		{
				min_=x;
				valid_=1;
		}
		damage(FL_AXIS_DAMAGE_ALL);
		update();

};

void Fl_Axis::adjust_range( double offset, double scale )
{
		min_ = proxy_min + fabs(proxy_max - proxy_min)*offset;
		max_ = min_ + fabs(proxy_max - proxy_min)*scale;
		update();
		redraw();
}

int Fl_Axis::update(){

    double _k=k_;
	double _q=q_;
    min_pos_=min_pos();
    max_pos_=max_pos();
    if (min_==max_)
        k_=0;
    else
		if(scale_ & FL_AXIS_LOG){
			k_=(max_pos_-min_pos_)/(log(max_)-log(min_));
			q_=min_pos_-k_*log(min_);
		}else{
			k_=(max_pos_-min_pos_)/(max_-min_);
			q_=min_pos_;
		}

    if((_k!=k_)||(_q!=q_))
        return 1;
    else
        return 0;
};

void Fl_Axis::rescale_move(int when, double  x){
    
	
    if((when&FL_AXIS_WHEN_MAX)&&(x>max_)){
		if(scale_ & FL_AXIS_LOG)
			min_ *=x/max_;
		else
			min_ += x-max_;
		max_=x;
        damage(FL_AXIS_DAMAGE_ALL);
        
    }
    if((when&FL_AXIS_WHEN_MIN)&&(x<min_)){
		if(scale_ & FL_AXIS_LOG)
			max_ *=x/min_;
        else
			max_ -= min_-x;
        min_=x;
        damage(FL_AXIS_DAMAGE_ALL);
    }
    valid_=1;
};

double Fl_Axis::position(double value){

    if (k_==0) return (min_pos_+max_pos_)/2;
	if(scale_ & FL_AXIS_LOG)
		return (int)(q_+k_*log(value));
	else
		return min_pos_+k_*(value-min_);
};

double Fl_Axis::value(double pos){
    if (max_==min_)
        return min_;
	if(scale_ & FL_AXIS_LOG)
		return exp(min_ +(pos-q_)/k_);
	else
		return (min_ +(pos-min_pos_)/k_);
};



int Fl_Axis::next_tick(int &tick_index, double &tick_value, int &tick_order, double &interval ){
        

	////////// I know snakes are evil creatures, but sometimes they work so there is such a serpent....
	////////// How many if...else can be in in a function? this is going to be a record in the G. book of r.

	static int number_per_order;
    double _tick_interval;
    double minor_number_;


	if(scale_ & FL_AXIS_LOG){   /////////////     begin logarithmic   /////////////////
		if (!interval){
            tick_order=(int)(floor(log10(min_)));
            if (tick_interval_!=0){
                interval=fabs(tick_interval_);
                number_per_order=(int)floor(10/interval+0.5);
            }else{
                number_per_order=(int)(abs(min_pos_-max_pos_)/(tick_separation_*log10(max_/min_)));
                if(number_per_order<=1){
                    label_step_=major_step_=3;
                    tick_order = 3*(tick_order/3);
                    interval=1;
                    number_per_order=0;
                }else{
                    int _no_per_o=number_per_order;
                    for(int i=NO_LOG_DEFAULTS-1;i>=0;i--){
                        major_step_=default_log_major_steps[i];
                        label_step_=default_log_label_steps[i];
                        interval=default_log_intervals[i];
                        number_per_order=(int)floor(10/interval+0.5);
                        if((10/interval)>=_no_per_o)
                            break;
                    }
                }
            }
            tick_index=number_per_order;
            tick_order--;
            tick_value=pow(10,tick_order);
            interval*=tick_value;
            
            if(!number_per_order){
                tick_order--;
                tick_value /=10;
                tick_index=1;
            }else
                tick_value *=10;
            return 1;
        }else{
            if (tick_value>(max_)){
                tick_index-=1;
                return 0;
            }else{
                if(number_per_order){
                    if(tick_index==number_per_order){
                        tick_order++;
                        interval*=10;
                        if(number_per_order<10){
                            tick_index=1;
                            tick_value=interval;
                        }else{
                            tick_index=2;
                            tick_value=2*interval;
                        }
                    }else{
                        tick_value +=interval;
                        tick_index++;
                    }
                }else{
                    tick_order++;
                    tick_index++;
                    tick_value *=10;
                }
                return 1;
            }
        }

	}else{     ///////////////     begin linear       //////////////////////
        if (!interval){
            minor_number_= (double)abs(min_pos_-max_pos_)/(double)tick_separation_;
            _tick_interval=tick_interval_;
            if (tick_interval_<0){
                interval=_tick_interval=-_tick_interval;
                tick_order=(int)floor(log10(_tick_interval));
            }else{
                if(_tick_interval!=0){
                    tick_order=(int)floor(log10(fabs(max_-min_)/minor_number_));
                    interval= pow(10,tick_order) * _tick_interval;
                }else
                    for(int i=NO_LIN_DEFAULTS-1;i>=0;i--){
                        tick_order=(int)floor(log10(fabs(max_-min_)/minor_number_));
                        interval= pow(10,tick_order)*(_tick_interval=default_lin_intervals[i]);
                        major_step_=default_lin_major_steps[i];
                        label_step_=default_lin_label_steps[i];
                        if(((max_-min_)/interval)>=minor_number_)
                            break;
                    }
            }
            tick_value = floor(minimum()/interval);
            tick_value *= interval;
            tick_index=(int) floor((tick_value /interval)+0.5);
            return 1;
        }else{
            if (tick_value>(max_)){
                tick_index=-1;
                return 0;
            }else{
                tick_value +=interval;
                tick_index++;
                return 1;
            }
        }
	}		/////   Uf, this is the end of the leg-less beast!   //////
};



void Fl_Axis::rescale(int when, double  x){
    if(!valid_){
        max_=x;
        min_=x;
        damage(FL_AXIS_DAMAGE_ALL);
        valid_=1;
        return;
    }

    if((when&FL_AXIS_WHEN_MAX)&&(x>max_)){
        max_=x;
        damage(FL_AXIS_DAMAGE_ALL);
    }
    if((when&FL_AXIS_WHEN_MIN)&&(x<min_)){
        min_=x;
        damage(FL_AXIS_DAMAGE_ALL);
    }
};

Fl_Axis::Fl_Axis(int x, int y, int w, int h, const char * label)
	: Fl_Box(x,y,w,h,label),
	  scale_(FL_AXIS_LIN), valid_(0), label_format_(0),
	  minor_grid_style_(FL_SOLID), major_grid_style_(FL_SOLID), label_grid_style_(FL_SOLID),
	  minor_grid_width_(0), major_grid_width_(0), label_grid_width_(0), 
	  minor_grid_dashes_(0), major_grid_dashes_(0),label_grid_dashes_(0),
	  grid_visible_(0), tick_interval_(MINOR_INTERVAL), tick_separation_(MINOR_SEPARATION),
	  tick_length_(0), tick_width_(0), major_step_(MAJOR_STEP),label_step_(LABEL_STEP),
	  axis_align_(FL_AXIS_BOTTOM),label_font_size_(LABEL_SIZE),
	  min_(0),max_(0),min_pos_(0),max_pos_(0),border_(AXIS_BORDER),axis_color_(FL_BLACK),
	  minor_grid_color_(FL_BLACK), major_grid_color_(FL_BLACK), label_grid_color_(FL_BLACK), 
	  label_font_face_(FL_HELVETICA)
	
{
	box(FL_NO_BOX);

	labelsize(LABEL_SIZE);
	
	AdjustRange.Wrap( this, &Fl_Axis::adjust_range );
};

Fl_Axis::~Fl_Axis()
{
	
}

}
