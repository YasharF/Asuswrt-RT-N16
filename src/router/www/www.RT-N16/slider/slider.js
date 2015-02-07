// The global array of objects that have been instanciated
if (!Bs_Objects) {var Bs_Objects = [];};


/**
* A Slider Component.
* 
* <b>Features:</b> 
* - IE6 and NS(Mozilla7) compliant.
* - Lets you add a slider to your page. It simulates a HTML-form text field; so your form-handling will not change.
* - You may use your customized images.
* - Able to attach onClick- and onChange-events to execute your JS-code when checked/changed.
*
* <b>Includes (+Dependencies):</b>
* <code>
*   <script type="text/javascript" src="/_bsJavascript/lib/LibCrossBrowser.js"></script>
*   <script type="text/javascript" src="/_bsJavascript/lib/EventHandler.js"></script>
*   <script type="text/javascript" src="/_bsJavascript/core/form/Bs_FormUtil.lib.js"></script>
*   <script type="text/javascript" src="/_bsJavascript/components/slider/Bs_Slider.class.js"></script>
* </code>
*
* <b>How to use:</b>
* 1. Have a look at the example (see example link below)
* 2. Create a function in the HTML-header called init(). Place the javascript code 
*    that instanciates and inits this component into init().
* 3. Place an 'onLoad' in your body-tag: e.g. <body onLoad="init();">
* 4. In the HTML body: Place a div- or span-tag with an unique ID where you want the component to be.
*
* <b>How it works:</b>
* - [After instanciating and initializing the object]. Call the drawInto([tag id]) method.
*   This will search the HTML code for a tag with the given id and insert HTML- and JS- code
*   dynamically (using DOM) to display the component and handle it.
* 
* <b>What is returned to the server:</b>
*<pre>
*  +------------------------+------------------------------------------------+
*  | [tag id] +"_value"     | int, The slider value                          |
*  +------------------------+------------------------------------------------+
* In versions 4.4 and before  
*  +--------------------------------------------+----------------------------+
*  | [varible name of the object] +"_value"     | int, The slider value      |
*  +--------------------------------------------+----------------------------+
*</pre>
*  This class generates a normal HTML text field and saves it's state in there.
*  It's not visible because it is set to display:none (invisible) but (when used in a form),
*  it will be submitted like any other HTML-form-field. To override this name see {@link fieldname}.
*
* <b>Event-Handling</b>: (see {@link attachOnClick()}, {@link attachOnChange()})
* - You can a callback function for certain events onto this component
*   (events are things like onClick, onChange ... aso) 
*
* Snippet 1) Attaching a callback function:
* <code>
*     var mySlider = new Bs_Slider();
*     mySlider.attachOnChange(yourCallbackFunctionName);
* </code>
* The function you pass should be able to receive 3 parameters.
* - The componentObj is an instance of this class and allows you to access it the 
*   component that triggered the event.
* - newValue is the new value that was set, new position is the new position in pixels.
* <code>
*     function yourCallbackFunctionName(componentObj, newValue, newPosition){
*       // do some stuff.
*     }
* </code>
*
* <b>Customized images:</b>
* - Set the path to your image-dir {@link imgDir}
* - Then use following methods setup the colorbar and to place the images you intend to use:
* {@link colorbar} 
* {@link setBackgroundImage()} 
* {@link setSliderIcon()}
* {@link setArrowIconLeft()}, {@link setArrowIconRight()}
* 
* @example example1.html
* @example example2.html
* 
* @author     andrej arn <andrej at blueshoes dot org>, sam blume <sam at blueshoes dot org>
* @package    javascript_components
* @subpackage slider
* @copyright  blueshoes.org
*/
function Bs_Slider(theFieldnamePrefix) {
  
	/**
  * Unique Object/Tag ID is initialized in the constuctor.
  * Based on this._id. Can be used in genarated JS-code as ID. Is set together 
  * from the  classname + this._id (see _constructor() code ).
  *
  * @access private
  * @var  string 
  */
  this._objectId;
	
  /**
  * When submitting the form, you'll receive the value under this name.
	* In other words you'll receive the data back to the server as if you had placed <br>
  * <code><input type=text name="[the fieldName]" id="[the fieldName]"  value="[the value]"></code><br>
	* into your HTML-form.
  *
  * Note:
  * - If not set the field name will be genarated as instanceName +"_value"  
  * 
	* @access public
	* @since  bs4.4 (before there was a bug and the field 'undefined' was used for all silders.)
	* @var    string
	*/
	this.fieldName;
	
	/**
	* If the whole slider is disabled or not.
	* @access private
	* @var    bool
	* @see    setDisabled()
	*/
	this._disabled = false;	

	/**
	* The slider direction (horizontal/vertical)
  *<pre>
	*   0 = horizontally (default)
	*   1 = vertically (currently not implemented)
  *</pre>
	* @access public
	* @var    int direction
	* @todo   implement vertically
	*/
  this.direction       = 0;
	
	/**
	* The width of the sliding part in pixel.
  * Note: This is not the full width. It excludes the input field and the push buttons.
	* @access public
	* @var    int (default is 100 pixel)
	*/
  this.width           = 100;
	
	/**
	* The height of the sliding part in pixel. 
	* This is not the full height. it excludes the input field and the push buttons.
	* @access public
	* @var    int (default is 20 pixel)
	*/
  this.height          = 20;
	
	/**
	* The lowest possible value. It may be lower than 0 afaik :-)
	* @access public
	* @var    int (default is 0)
	*/
  this.minVal          = 0;
	
	/**
	* The highest possible value.
	* @access public
	* @var    int  (default is 100)
	*/
  this.maxVal          = 100;
	
	/**
	* The default value (when the slider gets initialized).
	* Note:
  * - Can initially have alower or higher value than minVal/maxVal.
  *   This is a feature. You can detect if the used has set any value this way.
  *   Only  values in the range of minVal - maxVal can be set and are displayed.
	* @access public
	* @var    int 
	*/
  this.valueDefault      = 0;
  this.valueLeadDefault  = 0;
	
	/**
	* How much units to slide on an arrow click. 
	* If set to 0 then the arrows won't be displayed.
	* Something like 0.5 is possible aswell.
	* @access public
	* @var    int  (default is 1)
	* @see    this.wheelAmount
	*/
  this.arrowAmount     = 1;
	
	/**
	* if activated then a mouseover is already enough for the arrow to fire. it then 
	* does not need a click. it continues to fire until onmouseout.
	* @access public
	* @var    bool arrowMouseOver
	* @since  bs-4.6
	* @see    var this.arrowKeepFiringTimeout
	*/
	this.arrowMouseOver = false;
	
	/**
	* every this milliseconds the onChangeByArrow() events keep firing.
	* set to 0 if you don't want that.
	* @access public
	* @var    int arrowKeepFiringTimeout
	*/
	this.arrowKeepFiringTimeout = 10;
	
	/**
	* @access private
	* @var    bool _stopFireArrowFlag
	* @see    this.stopFireArrow()
	*/
	this._stopFireArrowFlag = false;
  
	/**
	* how much to scroll when the mouse wheel is used.
	* note: the mouse wheel is supported by ie only (as of 2004/03). 
	* @access public
	* @var    int wheelAmount
	* @see    this.arrowAmount
	* @since  bs-4.6
	*/
	this.wheelAmount = 5;
	
	
  /**
  * If and how the colorbar feature should be used.
  * 
  * - If it's a string then it will be interpreted as a css class. 
  * 
  * - If it's an array (hash) (or object with vars) the keys will be used to 
	*   generate style settings. 
	*   You can achieve the same results using both ways, except for 'widthDifference'.
  * 
  * - You can use both (array with css class as elemenet of the array) to 
  *   set additional css properties. I can't think of any, but maybe you do.
  * <pre>
  * if you give it an array, it can have these settings:
  *   KEY               DEFAULT
  *   cssClass          -nothing-
  *   color             'orange' if cssClass is not set, otherwise -nothing-
  *   height            -nothing-
  *   widthDifference   0
  *   offsetLeft        -nothing-
  *   offsetTop         -nothing-
  * 
  * coments:
  *   color:
  *     if you don't see the colorbar, you prolly have a cssClass defined 
  *     but neither there a color defined not in the array ('color' element).
  *   widthDifference:
  *     the width of the colorbar is always from the left side to where the 
  *     handle (slider) is. so the left side of the slider bar is colored.
  *     it may be (depending on your design) that you need to have the color 
  *     bar a little bit larger or smaller. you can set an amound of pixels 
  *     here, it may even be negative.
  *   offsetLeft:
  *     if you use a cssclass, you can specify something like left:5px;
  *     if you use arrows, you have to take the width of the left arrow 
  *     into account. if you use offsetLeft, you don't. this is done for you.
  * </pre>
  * 
  * anyway play with the values until you get what you want.
  * 
  * So if you want to use a css class and want to specify a widthDifference, 
  * do something like 
  * <code>
  *   arr = new Array()
  *   arr['cssClass'] = 'yourClass';
  *   arr['widthDifference'] = -5;
  * </code>
  * and you're done.
  * 
  * On the other hand it's handy if you don't have to create a css class and make 
  * sure it always ships with your file. well, enough of comments for this one.
  * 
  * @access public
  * @var    mixed  (array or string)
  */
  this.colorbar;
  this.leadingColorbar;
  
	/**
	* The z-index the slider will use. you may want to change this if you have 
	* elements on your page that overlap this slider (floating stuff or so).
	* @access public
	* @var int  (default is 1000)
	*/
  this.baseZindex      = 1000;
	
	/**
	* If you want to move the whole slider object with everything on the x-axis 
	* then set a value here. 
	* - example: -5 means move the whole thing  5 pixel to the left.
	* - example: 10 means move the whole thing 10 pixel to the right.
	* @access public
	* @var    int 
	* @see    moveY
	* @since  bs4.3
	*/
	this.moveX = 0;
	
	/**
	* If you want to move the whole slider object with everything on the y-axis 
	* then set a value here. 
	* - example: -5 means move the whole thing  5 pixel up the page.
	* - example: 10 means move the whole thing 10 pixel down the page.
	* @access public
	* @var    int 
	* @see    moveX
	* @since  bs4.3
	*/
	this.moveY = 0;
  
	/**
	* The base path for the image folder.
  * @deprecated use imgDir
  * @access private
  */
  this.imgBasePath;

	/**
	* The base path for the image folder.
	* 
	* Examples: (NOTE: The path has to end with a slash.)
  * - 'http://www.domain.com/foo/'  <= with domain
	* - '/my/image/path/'             <= absolute path without domain
	* - '../some/path/'               <= relative path
	* 
	* 
	* Default is: '/_bsJavascript/components/slider/img/'
	* Check this folder and the other folders around there for other styles.
	* 
	* @access public
	* @var    string
	*/
  this.imgDir  = '/_bsJavascript/components/slider/img/';
	
	/**
	* The name of the background image in the imgDir.
	* Set this using setBackgroundImage(), so look there.
	* @access private
	* @var    string 
	* @see    setBackgroundImage()
	*/
  this._bgImgSrc;
		
	/**
	* How a background-image should be repeated. it's a css property.
	* - example: 'no-repeat'
	* set this using setBackgroundImage(), so look there.
	* @access private
	* @var    string 
	* @see    setBackgroundImage()
	*/
  this._bgImgRepeat;
  
	/**
	* Additional css style string for the background image.
	* set this using setBackgroundImage(), so look there.
	* @access private
	* @var    string 
	* @see    setBackgroundImage()
	*/
	this._bgImgCssStyle;
	
	
	/**
	* @access private
	* @var    string 
	* @see    setBackgroundImageLeft()
	*/
  this._bgImgLeftSrc;
	
	/**
	* @access private
	* @var    int 
	* @see    setBackgroundImageLeft()
	*/
  this._bgImgLeftWidth;
	
	/**
	* @access private
	* @var    int 
	* @see    setBackgroundImageLeft()
	*/
  this._bgImgLeftHeight;
	
	/**
	* @access private
	* @var    string 
	* @see    setBackgroundImageRight()
	*/
  this._bgImgRightSrc;
	
	/**
	* @access private
	* @var    int 
	* @see    setBackgroundImageRight()
	*/
  this._bgImgRightWidth;
	
	/**
	* @access private
	* @var    int 
	* @see    setBackgroundImageRight()
	*/
  this._bgImgRightHeight;
	
	
	/**
	* The slider handle image name. (knob)
	* Set this using setSliderIcon().
	* @access private 
	* @var    int 
	* @see    setSliderIcon(), _sliderImgWidth, _sliderImgHeight
	*/
  this._sliderImgSrc;
	
	/**
	* The width of the slider handle image.
	* Set this using setSliderIcon().
	* @access private
	* @var    int 
	* @see    setSliderIcon(), _sliderImgSrc, _sliderImgHeight
	*/
  this._sliderImgWidth;
	
	/**
	* The height of the slider handle image.
	* Set this using setSliderIcon().
	* @access private
	* @var    int _sliderImgHeight
	* @see    setSliderIcon(), _sliderImgSrc, _sliderImgWidth
	*/
  this._sliderImgHeight;
    
	/**
	* Used to set a CSS class name for the slider container.
	* @access public
	* @var    string
	*/
  this.styleContainerClass;
	
	/**
	* Used to set a CSS class name for the value input field.
	* @access public
	* @var    string 
	*/
  this.styleValueFieldClass = 'smalltxt spanSliderField';
	
	/**
	* Used to set a CSS class name for the value text span.
	* @access public
	* @var    string 
	*/
  this.styleValueTextClass  = 'smalltxt spanSliderText';
  
	/**
	* The background color. hex code or named color.
	* examples: 'blue', 'green', '#000000'
	* @access public
	* @var    string 
	*/
  this.bgColor;	
	
	/**
	* The name of the left arrow icon.
	* @access private
	* @var    string 
	* @see    setArrowIconLeft()
	*/
  this._arrowIconLeftSrc;
  
	/**
	* The width of the left arrow icon.
	* @access private
	* @var    string  
	* @see    setArrowIconLeft()
	*/
  this._arrowIconLeftWidth   = 0;
  
	/**
	* The height of the left arrow icon.
	* @access private
	* @var    string  
	* @see    setArrowIconLeft()
	*/
  this._arrowIconLeftHeight  = 0;
  
	/**
	* A CSS style string to use in the image tag.
	* @access private
	* @var    string arrowIconLeftCssStyle
	* @see    setArrowIconLeft()
	* @todo   all
	*/
  this._arrowIconLeftCssStyle  = 0;

	/**
	* The name of the right arrow icon.
	* @access private
	* @var    string
	* @see    setArrowIconRight()
	*/
  this._arrowIconRightSrc;
  
	/**
	* The width of the right arrow icon.
	* @access private
	* @var    int 
	* @see    setArrowIconRight()
	*/
  this._arrowIconRightWidth  = 0;
  
	/**
	* The height of the right arrow icon.
	* @access private
	* @var    int 
	* @see    setArrowIconRight()
	*/
  this._arrowIconRightHeight = 0;
    
	/**
	* A CSS style string to use in the image tag.
	* @access private
	* @var    string
	* @see    setArrowIconRight()
	* @todo   all
	*/
  this._arrowIconRightCssStyle  = 0;
	
  /**
  * The step (interval) of the values. 
  * <pre>
  * Examples: 0, 1, 2, 3 ...     has an interval of 1
  *           0, 0.5, 1, 1.5 ... has an interval of 0.5
  * </pre>
  * with the current rounding it can go down to 2 digits after the dot, 
  * like 3.51.
  * 
  * @access public
  * @var    number  (int or real, whatever)
  */
  this.valueInterval   = 1;
  
  /**
  * Should we display the input field (with value) to the right? 
  * 
  * <pre>
  * 0 = no
  * 1 = show as text (not implemented yet, will be shown as #2)
  * 2 = show as input field (changable). this is the default.
  * 3 = show as text, onclick becomes an input field. (not all browsers support that, but 
  *     it's fancy :)
  * </pre>
  * 
  * The trick is to keep it invisible if set to false. we need it because it's 
  * the holder of the internal value. could be recoded, if you want to...
  * 
  * @access public
  * @var int useInputField
  * @see inputTextFieldEvent
  */
  this.useInputField = 2;
  
  /**
  * If {@link useInputField} is set to 3 we toggle. but when?
  * default is onMouseOver (over) but it can be set to 'click' 
  * (onClick). 
  * @access public
  * @var    string ('over' or 'click')
  * @see    useInputField
  */
  this.inputTextFieldEvent = 'over';
  
  /**
	* ?
  * @access private
	*/
  this.ctrl;
  
  /**
  * The real value we have. 
  * in the beginning it will be set to this.valueDefault.
  * <b>WARNING:</b> 
  * - don't mix this._valueInternal and this.valueInterval. 
  *   it's not only that one is private and the other is not, 
  *   it's that one is the internal value, the other is the STEP.
  *   maybe step would have been a better name. got that?
  * @access private
  * @var    double 
  * @see    getValue()
  */
  this._valueInternal;
  this._valueLeadInternal;

  /**
  * How the slider should be seen in the browser
  * @access private
  * @var    int
  * @see    setDisplay()
  */
  this._display         = 2;

  /**#@+
  * Ids and objects
  * @access private
  */

  this._arrowLeftContainerId;
  this._arrowLeftContainerObj;
  this._arrowLeftIconId;
  this._arrowLeftIconObj;
  
  this._arrowRightContainerId;
  this._arrowRightContainerObj;
  this._arrowRightIconId;
  this._arrowRightIconObj;
	
  this._valueContainerId;
	this._valueContainerObj;

  this._handleId;
  this._handleObj;
  this._valueFieldId;
  this._valueFieldObj;
  this._valueTextId;
  this._valueTextObj;
  this._slideBarId;
  this._slideBarObj;
  this._colorbarId;
  this._leadingColorbarId;
  this._colorbarObj;
  this._leadingColorbarObj;
  /**#@-*/
  
  /**#@+
  * positions and measurements in pixels
  * @access private
  */
  this._posUpperLeftX;
  this._posUpperLeftY;
  this._posSlideStart;
  this._posSlideEnd;
  /**#@-*/
  
  /**
  * That's this.width - this._sliderImgWidth. Slide area without the handle.
  * If you don't get this, draw a slider yourself. maybe you'll 
  * figure it out then. :-)
  * @access private
  * @var int
  */
  this._slideWidth;
  
  
  //attached (assigned) event functions (that exist in the global scope):
  
  /**
  * Array holding all the information about attached events. 
  * The structure can be like these:
  * <pre>
  * 1. attach a function directly
  *    syntax:  _attachedEvents['eventName'] = yourFunctionName;
  * 2. attach some javascript code
  *    syntax:  _attachedEvents['eventName'] = "yourCode();";
  *    example: _attachedEvents['eventName'] = "alert('hi'); callSomething('foo');";
  *    just keep in mind that you cannot use vars in that code, because when it 
  *    gets executed that will be another scope (unless the vars are global...)
  * 3. attach multiple things for the same event
  *    syntax:  _attachedEvents['eventName']    = new Array;
  *             _attachedEvents['eventName'][0] = yourFunctionName;
  *             _attachedEvents['eventName'][1] = "yourCode();";
  *    possible values for 'eventName' are:
  *     'onChange'
  * </pre>
  * 
  * @access private
  * @var    array  (hash, see above)
  * @see    attachEvent();
  */
  this._attachedEvents;
  
  /**
  * Fires whenever the value changes. that can happen by sliding, 
  * hitting arrow buttons or typing in values into the input field.
  * @access private
  * @var function
  * @see attachOnChange()
  */
  this.eventOnChange;
  
  /**#@+
  * Attached function for the slide start,  move and end event.
  * @access private
  * @var function  
  */
  this.slideStartCB;
  this.slideMoveCB;
  this.slideEndCB;
  /**#@-*/
  
	/**
	* the pseudo constructor.
	* @access private
	* @return void
	*/
	this._constructor = function(theFieldnamePrefix) {
  	// Put this instance into the global object instance list
    this._id = Bs_Objects.length;
    Bs_Objects[this._id] = this; 
    this._objectId = "Bs_Slider_"+this._id;
    // For copatibillity we still init "objectName"
    this.objectName = this._objectId;
    // In V4.4 the fieldname used to be given be the constuctor-parameter. So if
    // a paramter is passed in the constructor, we still use it as fieldname prefix
    if (typeof(theFieldnamePrefix) == 'string') {
      this.fieldName = theFieldnamePrefix + '_value';  
       this.objectName = theFieldnamePrefix;
    }
  }
   
  /**
  * Makes sure everything is ready to use. sets some default values if needed.
	* @access private
	* @return void
  */
  this._checkup = function() {
    if (typeof(this.minVal)     == 'undefined') this.minVal     = 0;
    if (typeof(this.maxVal)     == 'undefined') this.maxVal     = 10;
    if (typeof(this.valueDefault) == 'undefined') this.valueDefault = this.minVal;
    if (typeof(this.valueLeadDefault) == 'undefined') this.valueLeadDefault = 0;
    this._valueInternal = this.valueDefault;
    this._valueLeadInternal = this.valueLeadDefault;
    
    // This is for backward compatabillity only --sam 2003-06
    if (typeof(this.imgBasePath) == 'string')  this.imgDir = this.imgBasePath;
  }
  
	
	/**
	* loads a skin by its name. 
	* 
	* you can do the same with manual calls to setSliderIcon() etc, but this is quick and easy.
	* 
	* available skins:
	*   
	* 
	* @access public
	* @param  string skinName
	* @return bool
	* @since  bs-4.6
	*/
	this.loadSkin = function(skinName) {
		switch (skinName) {
			case 'winxp':
			case 'winxp-scrollbar-horizontal':
			  this.useInputField = 0;
				this.height        = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/winxp/';
			  this.setSliderIcon('horizontal_scrollbar_knob.gif', 17, 16);
			  this.setArrowIconLeft('horizontal_scrollbar_arrowLeft.gif', 17, 16);
			  this.setArrowIconRight('horizontal_scrollbar_arrowRight.gif', 17, 16);
				break;
			case 'winxp-scrollbar-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/winxp/';
			  this.setSliderIcon('vertical_scrollbar_knob.gif', 16, 17);
			  this.setArrowIconLeft('vertical_scrollbar_arrowUp.gif', 16, 17);
			  this.setArrowIconRight('vertical_scrollbar_arrowDown.gif', 16, 17);
				break;
			case 'osx':
			case 'osx-horizontal':
			  this.useInputField = 0;
				this.height        = 21;
			  this.imgDir        = '/_bsJavascript/components/slider/img/osx/';
			  this.setSliderIcon('horizontal_knob.gif', 17, 16);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setBackgroundImageLeft('horizontal_backgroundLeft.gif', 6, 21);
			  this.setBackgroundImageRight('horizontal_backgroundRight.gif', 6, 21);
				break;
			case 'osx-scrollbar-horizontal':
			  this.useInputField = 0;
				this.height        = 15;
			  this.imgDir        = '/_bsJavascript/components/slider/img/osx/';
			  this.setSliderIcon('horizontal_scrollbar_knobSmall.gif', 23, 15);
			  this.setBackgroundImage('horizontal_scrollbar_background.gif', 'repeat');
			  this.setArrowIconLeft('horizontal_scrollbar_arrowLeft.gif', 17, 15);
			  this.setArrowIconRight('horizontal_scrollbar_arrowRight.gif', 17, 15);
				break;
			case 'osx-scrollbar-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 15;
				this.imgDir        = '/_bsJavascript/components/slider/img/osx/';
			  this.setSliderIcon('vertical_scrollbar_knobSmall.gif', 15, 23);
			  this.setBackgroundImage('vertical_scrollbar_background.gif', 'repeat');
			  this.setArrowIconLeft('vertical_scrollbar_arrowUp.gif', 15, 17);
			  this.setArrowIconRight('vertical_scrollbar_arrowDown.gif', 15, 17);
				break;
			case 'os9':
			case 'os9-horizontal':
			  this.useInputField = 0;
				this.height        = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/os9/';
			  this.setSliderIcon('horizontal_scrollbar_knob.gif', 17, 16);
			  this.setBackgroundImage('horizontal_scrollbar_background.gif', 'repeat');
			  this.setArrowIconLeft('horizontal_scrollbar_arrowLeft.gif', 16, 16);
			  this.setArrowIconRight('horizontal_scrollbar_arrowRight.gif', 16, 16);
				break;
			case 'os9-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/os9/';
			  this.setSliderIcon('vertical_scrollbar_knob.gif', 16, 17);
			  this.setBackgroundImage('vertical_scrollbar_background.gif', 'repeat');
			  this.setArrowIconLeft('vertical_scrollbar_arrowUp.gif', 16, 16);
			  this.setArrowIconRight('vertical_scrollbar_arrowDown.gif', 16, 16);
				break;
			case 'opera7':
			case 'opera7-horizontal':
			  this.useInputField = 0;
				this.height        = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/opera7/';
			  this.setSliderIcon('horizontal_knob.gif', 19, 16);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setArrowIconLeft('horizontal_arrowLeft.gif', 16, 16);
			  this.setArrowIconRight('horizontal_arrowRight.gif', 16, 16);
				break;
			case 'opera7-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/opera7/';
			  this.setSliderIcon('vertical_knob.gif', 16, 19);
			  this.setBackgroundImage('vertical_background.gif', 'repeat');
			  this.setArrowIconLeft('vertical_arrowUp.gif', 16, 16);
			  this.setArrowIconRight('vertical_arrowDown.gif', 16, 16);
				break;
			case 'bob':
			case 'bob-horizontal':
				this.height        = 18;
			  this.imgDir        = '/_bsJavascript/components/slider/img/bob/';
			  this.setBackgroundImage('background.gif', 'no-repeat');
			  this.setSliderIcon('slider.gif', 13, 18);
			  //this.setArrowIconLeft('arrowLeft.gif', 16, 16);
			  //this.setArrowIconRight('arrowRight.gif', 16, 16);
			  this.colorbar = new Object();
			  this.colorbar['color']           = 'blue';
			  this.colorbar['height']          = 5;
			  this.colorbar['widthDifference'] = 0; //-12
			  this.colorbar['offsetLeft']      = 5;
			  this.colorbar['offsetTop']       = 9;
				break;
			case 'burp':
			case 'burp-horizontal':
			  this.useInputField = 0;
				this.height        = 11;
			  this.imgDir        = '/_bsJavascript/components/slider/img/burp/';
			  this.setSliderIcon('horizontal_knob.gif', 5, 11);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setArrowIconLeft('horizontal_arrowLeft.gif', 10, 11);
			  this.setArrowIconRight('horizontal_arrowRight.gif', 10, 11);
				break;
			case 'burp-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 11;
			  this.imgDir        = '/_bsJavascript/components/slider/img/burp/';
			  this.setSliderIcon('vertical_knob.gif', 11, 5);
			  this.setBackgroundImage('vertical_background.gif', 'repeat');
			  this.setArrowIconLeft('vertical_arrowUp.gif', 11, 10);
			  this.setArrowIconRight('vertical_arrowDown.gif', 11, 10);
				break;
			case 'ximian-industrial':
			case 'ximian-industrial-horizontal':
			  this.useInputField = 0;
				this.height        = 15;
			  this.imgDir        = '/_bsJavascript/components/slider/img/ximian_industrial/';
			  this.setSliderIcon('horizontal_knob.gif', 31, 15);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setArrowIconLeft('horizontal_arrowLeft.gif', 15, 15);
			  this.setArrowIconRight('horizontal_arrowRight.gif', 15, 15);
				break;
			case 'ximian-industrial-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 15;
			  this.imgDir        = '/_bsJavascript/components/slider/img/ximian_industrial/';
			  this.setSliderIcon('vertical_knob.gif', 15, 31);
			  this.setBackgroundImage('vertical_background.gif', 'repeat');
			  this.setArrowIconLeft('vertical_arrowUp.gif', 15, 15);
			  this.setArrowIconRight('vertical_arrowDown.gif', 15, 15);
				break;
			case 'smoothstreak':
			case 'smoothstreak-horizontal':
			  this.useInputField = 0;
				this.height        = 15;
			  this.imgDir        = '/_bsJavascript/components/slider/img/smoothstreak/';
			  this.setSliderIcon('horizontal_knob.gif', 31, 15);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setBackgroundImageLeft('horizontal_backgroundLeft.gif', 2, 15);
			  this.setBackgroundImageRight('horizontal_backgroundRight.gif', 2, 15);
			  this.colorbar = new Object();
			  this.colorbar['color']           = '#736D6B';
			  this.colorbar['height']          = 11;
			  this.colorbar['widthDifference'] = 0;
			  this.colorbar['offsetLeft']      = 0;
			  this.colorbar['offsetTop']       = 2;
				break;
			case 'smoothstreak-vertical':
				this.direction     = 1;
			  this.useInputField = 0;
				this.width         = 15;
			  this.imgDir        = '/_bsJavascript/components/slider/img/smoothstreak/';
			  this.setSliderIcon('vertical_knob.gif', 15, 31);
			  this.setBackgroundImage('vertical_background.gif', 'repeat');
			  this.setBackgroundImageLeft('vertical_backgroundTop.gif', 15, 2);
			  this.setBackgroundImageRight('vertical_backgroundBottom.gif', 15, 2);
				break;
			case 'aluminumalloyvolcanic':
			case 'aluminumalloyvolcanic-horizontal':
			  this.useInputField = 0;
				this.height        = 15;
			  this.imgDir        = '/_bsJavascript/components/slider/img/aluminumalloyvolcanic/';
			  this.setSliderIcon('horizontal_knob.gif', 15, 19);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setBackgroundImageLeft('horizontal_backgroundLeft.gif', 2, 19);
			  this.setBackgroundImageRight('horizontal_backgroundRight.gif', 2, 19);
				break;
			case 'yattacier3':
			case 'yattacier3-horizontal':
			  this.useInputField = 0;
				this.height        = 16;
			  this.imgDir        = '/_bsJavascript/components/slider/img/yattacier3/';
			  this.setSliderIcon('horizontal_knob.gif', 30, 16);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setBackgroundImageLeft('horizontal_backgroundLeft.gif', 1, 16);
			  this.setBackgroundImageRight('horizontal_backgroundRight.gif', 1, 16);
				break;
			case 'h2ogtk2':
			case 'h2ogtk2-horizontal':
			  this.useInputField = 0;
				this.height        = 17;
			  this.imgDir        = '/_bsJavascript/components/slider/img/h2ogtk2/';
			  this.setSliderIcon('horizontal_knob.gif', 30, 17);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setBackgroundImageLeft('horizontal_backgroundLeft.gif', 7, 17);
			  this.setBackgroundImageRight('horizontal_backgroundRight.gif', 7, 17);
				break;
			case 'h2ogtk2-scrollbar-horizontal':
			  this.useInputField = 0;
				this.height        = 17;
			  this.imgDir        = '/_bsJavascript/components/slider/img/h2ogtk2/';
			  this.setSliderIcon('horizontal_knob.gif', 30, 17);
			  this.setBackgroundImage('horizontal_background.gif', 'repeat');
			  this.setArrowIconLeft('horizontal_arrowLeft.gif', 15, 17);
			  this.setArrowIconRight('horizontal_arrowRight.gif', 15, 17);
				break;
			default:
				return false;
		}
		return true;
	}
	
	
	/**
	* Render function.
	* @access public
	* @param  string tagId (ID of the element in that we render the slider.)
	* @return void
	*/
  this.render = function(tagId) {
    this._checkup();
    
    this._containerId           = 'co'  + tagId;
    this._handleId              = 'po'  + tagId;
    this._arrowLeftContainerId  = 'alc'  + tagId;
    this._arrowLeftIconId       = 'ali'  + tagId;
    this._arrowRightContainerId = 'arc'  + tagId;
    this._arrowRightIconId      = 'ari'  + tagId;
    this._valueContainerId      = 'vc'  + tagId;
    this._valueFieldId          = 'vf'  + tagId;
		if (typeof(this.fieldName) == 'undefined') this.fieldName = tagId + '_value';
    this._valueTextId           = 'vt'  + tagId;
    this._slideBarId            = 'bar' + tagId;
    this._colorbarId            = 'cb'  + tagId;
    this._leadingColorbarId     = 'lcb'  + tagId;
    
    var divWidth      = this.width; // + this._sliderImgWidth;
    var divHeight     = this.height;
    //var completeWidth = this.width + this._arrowIconLeftWidth + this._arrowIconRightWidth + this._sliderImgWidth +15; //15 just to add some space.
    
    var out         = new Array();
    var outI        = 0;
    var localOffset = 0;
    
    //container begin
    out[outI++] = '<div id="' + this._containerId + '"';
    if (this.styleContainerClass) {
      out[outI++] = ' class="' + this.styleContainerClass + '"';
    }
    out[outI++] = ' style="position:relative;';
    if (this._display == 0) {
      out[outI++] = ' display:none;';
    } else if (this._display == 1) {
      out[outI++] = ' visibility:hidden;';
    }
    out[outI++] = ' onmousewheel="Bs_Objects['+this._id+'].onMouseWheel(); return false;"';
    out[outI++] = '">';
    
		out[outI++] = '<div';
    out[outI++] = ' onmousewheel="Bs_Objects['+this._id+'].onMouseWheel(); return false;"';
		out[outI++] = ' style="position:absolute; left:' + this.moveX + '; top:' + this.moveY + ';">';
		
		//knob
    out[outI++] = '<div style="position:absolute; display:none; z-index:5000;" id="' + this._handleId     + '">';
    out[outI++] = '<img name="bsslidericonname" src="' + this.imgDir + this._sliderImgSrc + '" border=0 width=' + this._sliderImgWidth + ' height=' + this._sliderImgHeight + '>';
    out[outI++] = '</div>';
    
    //arrow left
    if ((this.arrowAmount > 0) && this._arrowIconLeftSrc) {
      out[outI++] = '<div id="' + this._arrowLeftContainerId + '" style="position:absolute; left:' + localOffset + '; top:0;">';
			out[outI++] = '<a href="javascript:void(false);"';
			if (this.arrowMouseOver) {
				out[outI++] = ' onMouseOver="Bs_Objects['+this._id+'].onChangeByArrow(false, true); return false;"';
				out[outI++] = ' onMouseOut="Bs_Objects['+this._id+'].stopFireArrow(); return false;"';
			} else {
				//old code:
				out[outI++] = ' onClick="Bs_Objects['+this._id+'].onChangeByArrow(false); return false;"';
				//new code:
				//out[outI++] = ' onMouseDown="Bs_Objects['+this._id+'].onChangeByArrow(false, true); return false;"';
				//out[outI++] = ' onMouseUp="Bs_Objects['+this._id+'].stopFireArrow(); return false;"';
				//out[outI++] = ' onMouseOut="Bs_Objects['+this._id+'].stopFireArrow(); return false;"';
			}
			out[outI++] = '>';
			out[outI++] = '<img id="' + this._arrowLeftIconId + '" src="' + this.imgDir + this._arrowIconLeftSrc + '" border="0" width="' + this._arrowIconLeftWidth + '" height="' + this._arrowIconLeftHeight + '"';
			if (typeof(this.arrowIconLeftCssStyle) != 'undefined') {
				out[outI++] = ' style="' + this.arrowIconLeftCssStyle + '"';
			}
			out[outI++] = '>';
			out[outI++] = '</a></div>';
      localOffset += this._arrowIconLeftWidth;
    }
    
		//left background image
		if (typeof(this._bgImgLeftSrc) != 'undefined') {
			var tmpLeft = (this.direction == 0) ? localOffset : 0;
			var tmpTop  = (this.direction == 0) ? 0           : localOffset;
			out[outI++] = '<div style="position:absolute; left:' + tmpLeft + '; top:' + tmpTop + ';">';
			out[outI++] = '<img src="' + this.imgDir + this._bgImgLeftSrc + '" width="' + this._bgImgLeftWidth + '" height="' + this._bgImgLeftHeight + '" border="0">';
			out[outI++] = '</div>';
			localOffset += (this.direction == 0) ? this._bgImgLeftWidth : this._bgImgLeftHeight;
		}
		
    //leading colorbar
    if (this.leadingColorbar) {
      out[outI++] = '<div id="' + this._leadingColorbarId + '" onClick="return false;"';
      if (this.leadingColorbar['cssClass']) {
        out[outI++] = ' class="' + this.leadingColorbar['cssClass'] + '"';
      }
      out[outI++] = ' style="position:absolute; z-index:4000; width:0;';
      if ('undefined' != typeof(this.leadingColorbar['color'])) {
        out[outI++] = ' background-color:' + this.leadingColorbar['color'] + ';';
      } else if ('undefined' == typeof(this.leadingColorbar['cssClass'])) {
        out[outI++] = ' background-color:black;';
      }
      if ('undefined' != typeof(this.leadingColorbar['offsetLeft'])) {
        out[outI++] = ' left:' + (localOffset + this.leadingColorbar['offsetLeft']) + ';';
      }
      if ('undefined' != typeof(this.leadingColorbar['offsetTop'])) {
        out[outI++] = ' top:' + this.leadingColorbar['offsetTop'] + ';';
      }
      if ('undefined' != typeof(this.leadingColorbar['height'])) {
        out[outI++] = ' height:' + this.leadingColorbar['height'] + ';';
      }
      out[outI++] = '">';
      out[outI++] = '<img src="/_bsImages/spacer.gif" width="1" height="5"></div>';
    }
    

    //main layer
    out[outI++] = '<div id="' + this._slideBarId + '" onClick="Bs_Objects['+this._id+'].onChangeByClick(event);"';
		
		var tmpLeft = (this.direction == 0) ? localOffset : 0;
		var tmpTop  = (this.direction == 0) ? 0           : localOffset;
		out[outI++] = ' style="position:absolute; left:' + tmpLeft + '; top:' + tmpTop + '; width:' + divWidth + '; height: ' + divHeight + '; clip:rect(0 ' + divWidth + '  ' + divHeight + ' 0);';
		
    if (this.bgColor) {
      out[outI++] = 'background-color:' + this.bgColor + '; layer-background-color:' + this.bgColor + ';';
    }
    if (this._bgImgSrc) {
      out[outI++] = ' background-image: url(' + this.imgDir + this._bgImgSrc + '); background-repeat:' + this._bgImgRepeat + ';';
    }
		if (this._bgImgCssStyle) {
	    out[outI++] = this._bgImgCssStyle;
		}
    out[outI++] = '"></div>';

    //if (this.leadingColorbar)
    //	localOffset += (this.direction == 0) ? 20 : this.height;
	
    //colorbar
    if (this.colorbar) {
      out[outI++] = '<div id="' + this._colorbarId + '" onClick="Bs_Objects['+this._id+'].onChangeByClick(event);"';
      if (this.colorbar['cssClass']) {
        out[outI++] = ' class="' + this.colorbar['cssClass'] + '"';
      }
      out[outI++] = ' style="position:absolute; z-index:4000; width:0;';
      if ('undefined' != typeof(this.colorbar['color'])) {
        out[outI++] = ' background-color:' + this.colorbar['color'] + ';';
      } else if ('undefined' == typeof(this.colorbar['cssClass'])) {
        out[outI++] = ' background-color:orange;';
      }
      if ('undefined' != typeof(this.colorbar['offsetLeft'])) {
        out[outI++] = ' left:' + (localOffset + this.colorbar['offsetLeft']) + ';';
      }
      if ('undefined' != typeof(this.colorbar['offsetTop'])) {
        out[outI++] = ' top:' + this.colorbar['offsetTop'] + ';';
      }
      if ('undefined' != typeof(this.colorbar['height'])) {
        out[outI++] = ' height:' + this.colorbar['height'] + ';';
      }
      out[outI++] = '">';
      out[outI++] = '<img src="/_bsImages/spacer.gif" width="1" height="5"></div>';
    }
		
    localOffset += (this.direction == 0) ? this.width : this.height;
   
    //if (this.leadingColorbar)	
    // 	localOffset -= (this.direction == 0) ? 20 : this.height;

    
		
		//right background image
		if (typeof(this._bgImgRightSrc) != 'undefined') {
			var tmpLeft = (this.direction == 0) ? localOffset : 0;
			var tmpTop  = (this.direction == 0) ? 0           : localOffset;
			out[outI++] = '<div style="position:absolute; left:' + tmpLeft + '; top:' + tmpTop + ';">';
			out[outI++] = '<img src="' + this.imgDir + this._bgImgRightSrc + '" width="' + this._bgImgRightWidth + '" height="' + this._bgImgRightHeight + '" border="0">';
			out[outI++] = '</div>';
			localOffset += (this.direction == 0) ? this._bgImgRightWidth : this._bgImgRightHeight;
		}
		
    //arrow right
    if ((this.arrowAmount > 0) && this._arrowIconRightSrc) {
			var tmpLeft = (this.direction == 0) ? localOffset : 0;
			var tmpTop  = (this.direction == 0) ? 0           : localOffset;
      out[outI++] = '<div id="' + this._arrowRightContainerId + '" style="position:absolute; left:' + tmpLeft + '; top:' + tmpTop + ';">';
			out[outI++] = '<a href="javascript:void(false);"';
			if (this.arrowMouseOver) {
				out[outI++] = ' onMouseOver="Bs_Objects['+this._id+'].onChangeByArrow(true, true); return false;"';
				out[outI++] = ' onMouseOut="Bs_Objects['+this._id+'].stopFireArrow(); return false;"';
			} else {
				//old:
				out[outI++] = ' onClick="Bs_Objects['+this._id+'].onChangeByArrow(true); return false;"';
				//new:
				//out[outI++] = ' onMouseDown="Bs_Objects['+this._id+'].onChangeByArrow(true, true); return false;"';
				//out[outI++] = ' onMouseUp="Bs_Objects['+this._id+'].stopFireArrow(); return false;"';
				//out[outI++] = ' onMouseOut="Bs_Objects['+this._id+'].stopFireArrow(); return false;"';
			}
			out[outI++] = '>';
			out[outI++] = '<img id="' + this._arrowRightIconId + '" src="' + this.imgDir + this._arrowIconRightSrc + '" border="0" width="' + this._arrowIconRightWidth + '" height="' + this._arrowIconRightHeight + '"';
			if (typeof(this.arrowIconRightCssStyle) != 'undefined') {
				out[outI++] = ' style="' + this.arrowIconRightCssStyle + '"';
			}
			out[outI++] = '>';
			out[outI++] = '</a></div>';
      localOffset += this._arrowIconRightWidth;
    }
    
    //input field and text
    var styleValueFieldClass = (this.styleValueFieldClass) ? ' class="' + this.styleValueFieldClass + '"' : '';
    var styleValueTextClass  = (this.styleValueTextClass)  ? ' class="' + this.styleValueTextClass  + '"' : '';
    out[outI++] = '<div id="' + this._valueContainerId + '" style="position:absolute; left:' + localOffset + '; top:0px;">';
    if (this.useInputField == 1) {
      //view only
      out[outI++] = '<span' + styleValueTextClass + ' id="' + this._valueTextId + '">' + this.valueDefault  + '</span>';
      out[outI++] = '<input type="hidden" name="' + this.fieldName + '" id="' + this._valueFieldId + '" value="' + this.valueDefault + '">';
    } else if (this.useInputField == 2) {
      //edit
      out[outI++] = '<input type="text"' + styleValueFieldClass + ' onMouseOver="bsFormFieldSetFocusAndSelect(this, false);" name="' + this.fieldName + '" id="' + this._valueFieldId + '" value="' + this.valueDefault + '" size="2"';
      if (styleValueFieldClass == '') { //so it does not look *that* bad by default.
        out[outI++] = ' style="vertical-align:text-top; width:30; height:' + this.height + ';"';
      }
      out[outI++] = ' onKeyUp="Bs_Objects['+this._id+'].onChangeByInput(this.value, false);" onBlur="Bs_Objects['+this._id+'].onChangeByInput(this.value, true);">';
    } else if (this.useInputField == 3) {
      //view, start editmode on click
      out[outI++] = '<input type="text"' + styleValueFieldClass + ' onMouseOver="bsFormFieldSetFocusAndSelect(this, false);" name="' + this.fieldName + '" id="' + this._valueFieldId + '" value="' + this.valueDefault + '" size="2"';
      if (styleValueFieldClass == '') { //so it does not look *that* bad by default.
        out[outI++] = ' style="display:none; vertical-align:text-top; width:30; height:' + this.height + ';"';
      } else {
        out[outI++] = ' style="display:none;"';
      }
      out[outI++] = ' onKeyUp="Bs_Objects['+this._id+'].onChangeByInput(this.value, false);" onBlur="var _bss = Bs_Objects['+this._id+']; _bss.onChangeByInput(this.value, true); _bss.textboxEdit(false)">';
      out[outI++] = '<span' + styleValueTextClass + ' style="" id="' + this._valueTextId   + '" ';
      if (this.inputTextFieldEvent == 'click') {
        out[outI++] = 'onClick="Bs_Objects['+this._id+'].textboxEdit(true);"';
      } else {
        out[outI++] = 'onMouseOver="Bs_Objects['+this._id+'].textboxEdit(true);"';
      }
      out[outI++] = '>' + this.valueDefault  + '</span>';
    } else { //0
      out[outI++] = '<input type="hidden" name="' + this.fieldName + '" id="' + this._valueFieldId + '" value="' + this.valueDefault + '">';
    }
    out[outI++] = '</div>';
    
		out[outI++] = '</div>';
    
    //container end
    out[outI++] = '</div>';
    
    document.getElementById(tagId).innerHTML = out.join('');
    //document.getElementById(tagId).innerHTML = '<textarea>'+out.join('')+'</textarea>';
    //return;
		
    this._containerObj           = document.getElementById(this._containerId);
    this._handleObj              = document.getElementById(this._handleId);
    this._valueContainerObj      = document.getElementById(this._valueContainerId);
    this._arrowLeftContainerObj  = document.getElementById(this._arrowLeftContainerId);
    this._arrowLeftIconObj       = document.getElementById(this._arrowLeftIconId);
    this._arrowRightContainerObj = document.getElementById(this._arrowRightContainerId);
    this._arrowRightIconObj      = document.getElementById(this._arrowRightIconId);
    this._valueFieldObj          = document.getElementById(this._valueFieldId);
    this._valueTextObj           = document.getElementById(this._valueTextId);
    this._slideBarObj            = document.getElementById(this._slideBarId);
    this._colorbarObj            = document.getElementById(this._colorbarId);
    this._leadingColorbarObj     = document.getElementById(this._leadingColorbarId);
    
    //this._posSlideStart = (this.direction == 0) ? getDivLeft(this._slideBarObj) : getDivTop(this._slideBarObj); //30;
    
    // Subtracting 7 (this number by trial and error) from starting position so that slider can start sliding much before - had to do this when I did the proportional width changes
    this._posSlideStart = 16; //(this.direction == 0) ? getDivLeft(this._slideBarObj) - 7 : getDivTop(this._slideBarObj); //30;
    
    //this._slideWidth    = (this.direction == 0) ? this.width - this._sliderImgWidth : this.height - this._sliderImgHeight;
    // By trial and error I found if I don't subtract the slider image width from this.width, the sliders were working well.(especially where I need to move them in sync)
    this._slideWidth    = (this.direction == 0) ? this.width : this.height - this._sliderImgHeight;
    
    this._posSlideEnd   = this._posSlideStart + this._slideWidth;
    
    this._currentRelSliderPosX = this._posSlideStart;
    this._currentRelLeadSliderPosX = this._posSlideStart;
    
    // This next if block was added if a leading space is to be shown. Note that the "leading space" is actually part of the regular space we show.
    // It is just used so as to show it in a different color.
    if (this.valueLeadDefault > this.minVal) {
      //how many percent is valueLeadDefault from maxVal-minVal?
      var hundertPercent = this.maxVal - this.minVal;
      var myPercent      = (this.valueLeadDefault-this.minVal) * 100 / hundertPercent;
      //now how much is that from the given length?
      this._currentRelLeadSliderPosX += (myPercent * this._slideWidth / 100);
      this._updateLeadColorbar(this._currentRelLeadSliderPosX);
    }

    if (this.valueDefault > this.minVal) {
      //how many percent is valueDefault from maxVal-minVal?
      var hundertPercent = this.maxVal - this.minVal;
      var myPercent      = (this.valueDefault-this.minVal) * 100 / hundertPercent;
      //now how much is that from the given length?
      this._currentRelSliderPosX += (myPercent * this._slideWidth / 100);
      //alert("Start : " + this._posSlideStart + " percentwidth : " + (myPercent * this._slideWidth / 100) + " current pos :" + this._currentRelSliderPosX)
      this._updateColorbar(this._currentRelSliderPosX);
    }
		if (this.direction == 0) {
	    this._handleObj.style.left = this._currentRelSliderPosX;
		} else {
	    this._handleObj.style.top  = this._currentRelSliderPosX;
		}
    this._handleObj.style.display = 'block';
    
    //handleObj = handlespan
    temp = ech_attachMouseDrag(this._handleObj,this.slideStart,null,this.slideMove,null,this.slideEnd,null,null,null);
    temp = temp.linkCtrl(getDivImage('','bsslidericonname'));
    this.ctrl           = temp;
    this.ctrl.sliderObj = this; //add back reference
		
    var x = getDivLeft(this._handleObj);
    var y = getDivTop(this._handleObj);
    y = 0; //z3b
    
    //alert(x);
    //alert(y); return;
    
    if (this.direction == 0) {   // horizontal
      this.ctrl.minX = this._posSlideStart;
      this.ctrl.maxX = this._posSlideEnd;
      this.ctrl.minY = y;
			this.ctrl.maxY = y;
    } else {      // vertical
      this.ctrl.minX = x;
      this.ctrl.maxX = x;
      this.ctrl.minY = this._posSlideStart;
			this.ctrl.maxY = this._posSlideEnd;
    }
    /*
    if (this.direction == 0) {   // horizontal
      this.ctrl.minX = x-Math.abs((this.valueDefault - this.minVal) * this.width / (this.maxVal - this.minVal));
      this.ctrl.maxX = x+Math.abs((this.maxVal - this.valueDefault) * this.width / (this.maxVal - this.minVal));
      this.ctrl.minY = y; this.ctrl.maxY = y;
    } else {      // vertical
      this.ctrl.minX = x; this.ctrl.maxX = x;
      this.ctrl.minY = y-Math.abs((this.valueDefault - this.minVal) * this.width / (this.maxVal - this.minVal));
      this.ctrl.maxY = y+Math.abs((this.maxVal - this.valueDefault) * this.width / (this.maxVal - this.minVal));
    }
    */
    
  }
  
  
  /**
  * Renders the checkbox component and places it into the page.
	* 
  * @access public
  * @param string tagId Id of the tag. (Use <div> or <span> to hold the ID)
	* @return void
  */
  this.drawInto = function(tagId) {
    this.render(tagId);
		if (this._disabled) this.setDisabled(true);
  }
  
  /**
  * DEPRECATED.
	* Use drawInto()
	* @deprecated use drawInto()
  * @param string tagId (id of the span tag.)
  * @see drawInto()
	* @return void
	*/
  this.draw = function(tagId) {
    this.render(tagId);
		if (this._disabled) this.setDisabled(true);    
  }

  /**
  * Attaches an event like onChange, onMouseOver, onClickCaption ... a.s.o.
  * Supported events are:
  * - 'onChange'
  *
  * @access public
  * @param  string trigger (for example 'onChange')
  * @param  function A globalFunctionName (No string)
  * @return void
  * @see    _attachedEvents
  */
  this.attachEvent = function(trigger, yourEvent) {
    if (typeof(this._attachedEvents) == 'undefined') {
      this._attachedEvents = new Array();
    }
    
    if (typeof(this._attachedEvents[trigger]) == 'undefined') {
      this._attachedEvents[trigger] = new Array(yourEvent);
    } else {
      this._attachedEvents[trigger][this._attachedEvents[trigger].length] = yourEvent;
    }
  }
  
  /**
  * Tells if an event is attached for the trigger specified. 
  * @access public
  * @param  string trigger
  * @return bool
  */
  this.hasEventAttached = function(trigger) {
    return (this._attachedEvents && this._attachedEvents[trigger]);
  }
  
  /**
  * Fires the events for the trigger specified.
  * (used internally, but feel free to trigger events yourself...)
  * @access public
  * @param  string trigger (for example 'onClickCaption')
  * @return void
  */
  this.fireEvent = function(trigger) {
    if (this._attachedEvents && this._attachedEvents[trigger]) {
      var e = this._attachedEvents[trigger];
      if ((typeof(e) == 'string') || (typeof(e) == 'function')) {
        e = new Array(e);
      }
      for (var i=0; i<e.length; i++) {
        if (typeof(e[i]) == 'function') {
          e[i](this);
        } else if (typeof(e[i]) == 'string') {
          eval(e[i]);
        } //else murphy
      }
    }
  }
  
  
  /**#@+
  * Attache a global JS-fuction that will be called.
  * @access public
  * @param  function functionName (not a string!)
  */
  this.attachOnChange = function(functionName) {
    this.eventOnChange = functionName;
  }

  this.attachOnSlideStart = function(functionName) {
    this.slideStartCB = functionName;
  }
  this.attachOnSlideMove = function(functionName) {
    this.slideMoveCB = functionName;
  }
  this.attachOnSlideEnd = function(functionName) {
    this.slideEndCB = functionName;
  }
  /**#@-*/
    
  /**#@+
  * Attache a global JS-fuction that will be called.
  * @access public
  * @param  function func (not a string!)
  */
  this.attachOnArrow = function(functionName) {
    this.eventOnArrow = functionName;
  }
  this.attachOnInputChange = function(functionName) {
    this.eventOnInputChange = functionName;
  }
  this.attachOnInputBlur = function(functionName) {
    this.eventOnInputBlur = functionName;
  }
  /**#@-*/
  
	
  /**
  * Sets the knob icon to be used by the slider.
  * @access public
  * @param  string imgName (on path)
  * @param  int  width if image in pixel
  * @param  int  height if image in pixel
  * @see imgDir
  * @return void
  */
  this.setSliderIcon = function(imgName, width, height) {
    this._sliderImgSrc    = imgName;
    this._sliderImgWidth  = width;
    this._sliderImgHeight = height;
  }
  
	/**
	* sets the arrow icon left/top (depending on direction horizontal/vertical).
	* @access public
	* @param  string imgName
	* @param  int width
	* @param  int height
	* @return void
	*/
  this.setArrowIconLeft = function(imgName, width, height) {
    this._arrowIconLeftSrc    = imgName;
    this._arrowIconLeftWidth  = width;
    this._arrowIconLeftHeight = height;
  }
	
	/**
	* sets the arrow icon right/bottom (depending on direction horizontal/vertical).
	* @access public
	* @param  string imgName
	* @param  int width
	* @param  int height
	* @return void
	*/
  this.setArrowIconRight = function(imgName, width, height) {
    this._arrowIconRightSrc    = imgName;
    this._arrowIconRightWidth  = width;
    this._arrowIconRightHeight = height;
  }
  
  /**
  * Sets a background image.
	* 
	* possible values for the repeat css property: 
	*   repeat    => the background image repeats on both axis
	*   repeat-x  => the bg image repeats on the x-axis only. 
	*   repeat-y  => the bg image repeats on the y-axis only.
	*   no-repeat => the bg image does not repeat; it only shows once.
	* 
  * @access public
  * @param  string src
  * @param  string repeat (read above)
	* @param  string cssStyle (additional css style string, eg "left:4px;" to indent 4px from the left. since bs-4.6)
  * @return void
	* @see    setBackgroundImageLeft(), setBackgroundImageRight()
  */
  this.setBackgroundImage = function(src, repeat, cssStyle) {
    this._bgImgSrc        = src;
    this._bgImgRepeat     = repeat;
		this._bgImgCssStyle   = cssStyle;
  }
	
  /**
  * Sets a background image to use on the left side. see the examples.
  * @access public
	* @param  string imgName
	* @param  int width
	* @param  int height
  * @return void
	* @see    setBackgroundImage(), setBackgroundImageRight()
  */
  this.setBackgroundImageLeft = function(imgName, width, height) {
    this._bgImgLeftSrc    = imgName;
    this._bgImgLeftWidth  = width;
    this._bgImgLeftHeight = height;
  }
	
  /**
  * Sets a background image to use on the right side. see the examples.
  * @access public
	* @param  string imgName
	* @param  int width
	* @param  int height
  * @return void
	* @see    setBackgroundImage(), setBackgroundImageLeft()
  */
  this.setBackgroundImageRight = function(imgName, width, height) {
    this._bgImgRightSrc    = imgName;
    this._bgImgRightWidth  = width;
    this._bgImgRightHeight = height;
  }
	
  
  /**
  * How the slider should be seen in the browser. 
  * <pre>
  *   (rtfm about css)
  *   0 = display:none
  *   1 = visibility:hidden
  *   2 = display:block
  * </pre>
  * 
  * @access public
  * @param  int display
  * @see    _display
  */
  this.setDisplay = function(display) {
    this._display = display;
    if (this._containerObj) {
      switch (display) {
        case 0:
          this._containerObj.style.display = 'none';
          break;
        case 1:
          this._containerObj.style.visibility = 'hidden';
          break;
        case 2:
          //we need to activate both
          this._containerObj.style.visibility = 'visible';
          this._containerObj.style.display = 'block';
          break;
        default: 
          //user set an invalid value.
      }
    }
  }
  /**
	* Disables (or re-enables) the whole slider.
	* 
	* if the param b is not specified, the current disabled mode will be toggled (inverted).
	* 
	* @access public
	* @param  bool b (true=disabled, false=enabled)
	* @since  bs4.4
	* @status experimental (or still at work)
	*/
	this.setDisabled = function(b) {
		if (typeof(b) == 'undefined') b = !this._disabled;
		
		if (b) {
			//be aware that the filters don't work on all elements, in all cases. there's not much i can do.
			//it once didn't work in a div, but then it worked in a table at the same place.
			//now it does not work on a span. well... not a killer feature.
			var filter = 'progid:DXImageTransform.Microsoft.BasicImage(grayScale=1); progid:DXImageTransform.Microsoft.BasicImage(opacity=.5)';
			var cursor = 'default';
		} else {
			var filter = null;
			var cursor = 'hand'; //moz == pointer
		}
		//try {
			var t = new Array(
				this._containerId, this._arrowLeftContainerId, this._arrowRightContainerId, 
				this._valueFieldId, this._valueTextId, 
				this._slideBarId, this._colorbarId, this._handleId
			); //this._valueContainerId, 
			for (var i=0; i<t.length; i++) {
				var elm = document.getElementById(t[i]);
				if (elm != null) elm.style.filter = filter;
			}
			var elm = document.getElementById(this._arrowLeftIconId);
			if (elm != null) elm.style.cursor = cursor;
			
			var elm = document.getElementById(this._arrowRightIconId);
			if (elm != null) elm.style.cursor = cursor;
			
			var elm = document.getElementById(this._valueFieldId);
			if (elm != null) elm.disabled = b;
			
		//} catch (e) {
		//}
		
		this._disabled = b;
	}
  
  /**
  * Returns the current silder value.
  * @access public
  * @return double
	* @see    getValueInPercent()
  */
  this.getValue = function() {
    return this._valueInternal;
  }
  
	/**
	* returns the current slider value in percent (compared to min/max value).
	* @access public
	* @return double (0 - 100)
	* @see    getValue()
	* @since  bs-4.6
	*/
	this.getValueInPercent = function() {
		var range   = Math.abs(this.maxVal - this.minVal);
		var percent = this._valueInternal / range * 100;
		return percent;
	}
	
	
  /**
  * Returns the current slider relative position in pixel.
  * @access public
  * @return int
  */
  this.getSliderPos = function() {
    var absLeng = (this.direction==0) ? getDivLeft(this.ctrl.div) - this.ctrl.minX : getDivTop (this.ctrl.div) - this.ctrl.minY;
    var absRang = this.maxVal - this.minVal;
    //alert("975: absLeng:" +absLeng+"  absRang:"+absRang+"  _slideWidth:"+this._slideWidth);
    return (absLeng * absRang/this._slideWidth) + this.minVal;
  }

    
  /**
  * Fires when the user slides.
  * @access private
  */
  this.onChangeBySlide = function() {
    if (this._disabled) return;
    var newPos = this._getNewLocationFromCursor();
    var val = this._getValueByPosition(newPos);
    val     = this._roundToGrid(val);
    if (val != this._valueInternal) {
      this._valueInternal = val;
      this.updateHandle(newPos);
      this.updateValueField(val);
      this.updateValueText(val);
      this._updateColorbar(newPos);
      if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val, newPos);
      this.fireEvent('onChange');
    }
  }
  
  /**
  * If a browser does not support that way of changing the slider value, 
  * nothing gets done.
  * @access public
  * @param  object event (the event object)
  * @return void
  */
  this.onChangeByClick = function(event) {
    if (this._disabled) return;
    var newPos = 0;
    if ('undefined' != typeof(event.offsetX)) {
      newPos = (this.direction == 0) ? event.offsetX + this._posSlideStart : event.offsetY + this._posSlideStart;
    } else if ('undefined' != typeof(event.layerX)) {
      newPos = (this.direction == 0) ? event.layerX + this._posSlideStart  : event.layerY  + this._posSlideStart;
    } else {
      return; //not supported.
    }
    
    var val = this._getValueByPosition(newPos);
    val     = this._roundToGrid(val);
    if (val != this._valueInternal) {
      this._valueInternal = val;
      this.updateHandle(newPos);
      this.updateValueField(val);
      this.updateValueText(val);
      this._updateColorbar(newPos);
      if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val, newPos);
      this.fireEvent('onChange');
    }
  }
  
  /**
  * Fires when the user inputs a new value into the input field.
  * @access public
  * @param  string val (new value)
  * @param  bool isBlur (if it's a onChange event set to false, on an onBlur event set to true.)
  * @return void
  */
  this.onChangeByInput = function(val, isBlur) {
    if (this._disabled) return;
    if (val == '') {
      val = this.minVal;
    }
    val = this._roundToGrid(val);
    var newPos = this._getPositionByValue(val);
    if (val != this._valueInternal) {
      this._valueInternal = val;
      this.updateHandle(newPos);
      this._updateColorbar(newPos);
      if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val, newPos);
      this.fireEvent('onChange');
      if (isBlur) { //the user may still be typing. don't fuck his typing until he left the field.
        this.updateValueField(val);
        this.updateValueText(val);
      }
    } else if (isBlur) {
      //it's possible that the field val is "" (empty) and now the user left that field.
      //we it is still empty, we got the blur event, and need to update the field/text values:
      this.updateValueField(val);
      this.updateValueText(val);
    }
  }
  
  /**
  * @access public
  * @param  bool leftOrRight (false=left, true=right. as with politics. :)
	* @param  bool keepFiring  (if true then it sets a timeout to fire again, until this cycle is exited using stopFireArrow().)
	* @param  bool loopCall    (used internally only on calls from setTimeout().)
  */
  this.onChangeByArrow = function(leftOrRight, keepFiring, loopCall) {
		if (!loopCall) this._stopFireArrowFlag = false;
		
		if (this._stopFireArrowFlag) return;
    if (this._disabled) return;
		
    var val = parseFloat(this._valueInternal);
    if (leftOrRight) {
      val += this.arrowAmount; //right arrow
    } else {
      val -= this.arrowAmount; //left arrow
    }
    val = this._roundToGrid(val);
    if (val != this._valueInternal) {
      this._valueInternal = val;
      var newPos = this._getPositionByValue(val);
      this.updateHandle(newPos);
      this.updateValueField(val);
      this.updateValueText(val);
      this._updateColorbar(newPos);
      if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val, newPos);
      this.fireEvent('onChange');
    }
		
		if (keepFiring) {
			if (!this._stopFireArrowFlag && (this.arrowKeepFiringTimeout > 0)) {
				setTimeout('Bs_Objects[' + this._id + '].onChangeByArrow(' + leftOrRight + ', ' + keepFiring + ', true);', this.arrowKeepFiringTimeout);
			}
		}
  }
  
	
	/**
	* is called when the mouse wheel is used over the slider.
	* @access public (used internally, you don't need that)
	* @return void
	*/
	this.onMouseWheel = function() {
		/*
		if (event.wheelDelta > 0) {
		  this.onChangeByArrow(false);
		} else if (event.wheelDelta < 0) {
		  this.onChangeByArrow(true);
		}
		*/
		
    if (this._disabled) return;
		
    var val = parseFloat(this._valueInternal);
    if (event.wheelDelta > 0) {
      val -= this.wheelAmount;
    } else {
      val += this.wheelAmount;
    }
    val = this._roundToGrid(val);
    if (val != this._valueInternal) {
      this._valueInternal = val;
      var newPos = this._getPositionByValue(val);
      this.updateHandle(newPos);
      this.updateValueField(val);
      this.updateValueText(val);
      this._updateColorbar(newPos);
      if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val, newPos);
      this.fireEvent('onChange');
    }
	}
	
	
	/**
	* stops a loop of firing onChangeByArrow().
	* @access public
	* @return void
	* @since  bs-4.6
	*/
	this.stopFireArrow = function() {
		this._stopFireArrowFlag = true;
	}
	
  /**  
  * Sets a new value.
  * @access public
  * @param  string val (new value)
  * @return void
  */
  this.setValue = function(val) {
    val = this._roundToGrid(val);
    var newPos = this._getPositionByValue(val);
    if (val != this._valueInternal) {
      this._valueInternal = val;
      this.updateHandle(newPos);
      this._updateColorbar(newPos);
      if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val, newPos);
      this.fireEvent('onChange');
      this.updateValueField(val);
      this.updateValueText(val);
    }
  }

  /**  
  * Sets a new value for the leading space.
  * @access public
  * @param  string val (new value)
  * @return void
  */
  this.setLeadValue = function(val) {
    val = this._roundToGrid(val);
    var newPos = this._getPositionByValue(val);
    this._updateLeadColorbar(newPos);
    // Have to adjust the left and width of the colorbar as well.
    newPos = this._getPositionByValue(this._valueInternal);
    this._updateColorbar(newPos);
  }

  /**
	* DEPRECATED use setValue(), this method name was confusing.
	* 
  * Use this to set a new value.
	* 
  * --sam the name is confusing, it's a set-function
	* --andrej: yes, true. I have added setValue().
	* 
  * Note: The value you set is left "as is", thus it is *not* rounded to the next grid.
  *       "Grid rounding" will occure on change (see onChangeByArrow, onChangeByInput, ...). 
  * @access public
  * @param  string val (new value)
  * @return void
	* @depreacted use setValue()
  */
  this.onChangeByApi = function(val) {
		this.setValue(val);
  }

  
  /**
  * Updates the colorbar visually.
  * @access private
  * @param  int newPos (at least i think it's an int.)
  * @return void
  */
  this._updateColorbar = function(newPos) {
//    alert(this._slideBarObj);
//    alert(this._colorbarObj);
    
//    if (this._colorbarObj == null) alert('hi');
//      alert(typeof(this._colorbarObj));

/*
    if (this._colorbarId && !this._colorbarObj) {
      //that happens with mozilla :/
      this._colorbarObj   = document.getElementById(this._colorbarId);
      alert(typeof(this._colorbarObj));
    }
*/
    
    if (this._colorbarObj) {
      var newWidth = newPos + this.colorbar['widthDifference']; //-10;
      if (this.leadingColorbar){
      	//newWidth = newWidth -  20;
      	newWidth = newWidth -  getDivWidth(this._leadingColorbarObj);
      	// Recalculate offsetleft of the layer wrt to the leading colorbar div
      	this._colorbarObj.style.left = getDivLeft(this._leadingColorbarObj) + getDivWidth(this._leadingColorbarObj)
      	
      }
      	
      if (newWidth < 0) newWidth = 0;
      this._colorbarObj.style.width = newWidth;
			
			if (typeof(this.colorbar['color2']) != 'undefined') {
				//fading feature used.
				var percent  = this.getValueInPercent();
				var newColor = mixColor(this.x, this.colorbar['color2'], percent);
				document.getElementById(this._colorbarId).style.backgroundColor = newColor;
			}
    }
  }
  
  /**
  * Updates the leading colorbar visually.
  * @access private
  * @param  int newPos (at least i think it's an int.)
  * @return void
  */
  this._updateLeadColorbar = function(newPos) {
    
    
    if (this._leadingColorbarObj) {
      var newWidth = newPos + this.leadingColorbar['widthDifference']; //-10;
      if (newWidth < 0) newWidth = 0;
      this._leadingColorbarObj.style.width = newWidth;
    }
  }
  
  
  /**
  * Calculates the value based on the given position.
  * @access private
  * @param  int pos
  * @return double
  */
  this._getValueByPosition = function(pos) {
		if (this.direction == 0) {
	    pos -= this.ctrl.minX;
  	  var hundertPercent = this.ctrl.maxX - this.ctrl.minX;
		} else {
	    pos -= this.ctrl.minY;
  	  var hundertPercent = this.ctrl.maxY - this.ctrl.minY;
		}
    var myPercent      = pos / hundertPercent;
    var val            = this.minVal + ((this.maxVal - this.minVal) * myPercent);
    return val;
  }
  
  /**
  * Calculates the position based on the given value.
  * @access private
  * @param  double val
  * @return int
  */
  this._getPositionByValue = function(val) {
    val = val - this.minVal; //since 4.3, needed. was buggy before if minVal was not 0.
    var hundertPercent = this.maxVal - this.minVal;
    var myPercent      = val / hundertPercent;
		if (this.direction == 0) {
	    var pos = this.ctrl.minX + ((this.ctrl.maxX - this.ctrl.minX) * myPercent);
		} else {
	    var pos = this.ctrl.minY + ((this.ctrl.maxY - this.ctrl.minY) * myPercent);
		}
    return pos;
  }
  
  /**
  * Parse and round value to next grid defined by this.valueInterval, 
	* and check lower/upper bounds.
  * @access private
  * @param  string val
  * @return float
  */
  this._roundToGrid = function(val) {
    val = parseFloat(val);
    if (isNaN(val)) return this.minVal;
    
    val = Math.round(val / this.valueInterval) * this.valueInterval;
    // Js has some odd rounding problems somewhere at 10^-8. To get rid of it we do: 
    val = Math.round(val*10000)/10000;
    
    //check upper/lower bounds
    if (val < this.minVal) val = this.minVal;
    if (val > this.maxVal) val = this.maxVal;
		
    return val;
  }
  
  /**
  * Returns the new handle pos.
  * @access private
  * @return int
  */
  this._getNewLocationFromCursor = function() {
    var ox = this._posEventSlideStartX;
    var oy = this._posEventSlideStartY;
    switch (this.direction) {
      case 0: // horizontal
        var t = this.ctrl.pageX - ox;
        var x = parseInt(this._posObjSlideStartX) + t;
        if (x > this.ctrl.maxX) x = this.ctrl.maxX;
        if (x < this.ctrl.minX) x = this.ctrl.minX;
        return x;
        /*
        if (this.ctrl.pageX > this.ctrl.maxX) {
          x=this.ctrl.maxX;
        } else if (this.ctrl.pageX < this.ctrl.minX) {
          x=this.ctrl.minX;
        } else {
          x = this.ctrl.pageX;
          if (x < this.ctrl.minX) x = this.ctrl.minX;
          if (x > this.ctrl.maxX) x = this.ctrl.maxX;
        }
        return x;
        break;
				*/
      case 1: // vertical
        var t = this.ctrl.pageY - oy;
        var y = parseInt(this._posObjSlideStartY) + t;
        if (y > this.ctrl.maxY) y = this.ctrl.maxY;
        if (y < this.ctrl.minY) y = this.ctrl.minY;
        return y;
    }
  }
  
  /**
  * Slides, in other words "updates the handle/knob".
  * @access private
  * @deprecated use updateHandle()
  * @param  int newPos
  * @return void
  */
  this.updatePointer = function(newPos) {
    this.updateHandle(newPos);
  }
  
  /**
  * Slides to new relative position, in other words "moves the handle/knob".
  * @access public
  * @param  int newPos in pixel
  * @return void
  */
  this.updateHandle = function(newPos) {
		if (this.direction == 0) {
	    this._currentRelSliderPosX = newPos;
  	  this.ctrl.div.style.left   = newPos;
		} else {
	    this._currentRelSliderPosX = newPos;
  	  this.ctrl.div.style.top    = newPos;
		}
    return;
		/*
    switch (this.direction) {
      case 0: // horizontal
        moveDivTo(this.ctrl.div, newPos, getDivTop(this.ctrl.div));
        break;
      case 1: // vertical
        moveDivTo(this.ctrl.div, getDivTop(this.ctrl.div), newPos);
        break;
    }
		*/
  }
  
  /**
  * Updates the value of the input field.
  * @access public
  * @param  mixed val (string or number)
  * @return void
  * @see    updateValueText()
  */
  this.updateValueField = function(val) {
    if (this._valueFieldObj) {
      this._valueFieldObj.value = val;
    }
  }
  
  /**
  * Updates the value of the text box.
  * @access public
  * @param  mixed val (string or number)
  * @return void
  * @see    updateValueField()
  */
  this.updateValueText = function(val) {
    if (this._valueTextObj) {
      //alert(this._valueTextObj.innerText);
      //alert(this._valueTextObj.innerHTML);
      //alert(val);
      this._valueTextObj.innerHTML = val;
      //alert(this._valueTextObj.innerText);
    }
  }
  
  /**
  * @access private
  */
  this.arrowOnClick = function() {
  }
  
  /**
  * fires after the value has changed. fires a lot when sliding, also 
  * fires after an arrow click or input change.
  * @access private
  * @param int val (new value)
  */
  this.onChange = function(val) {
		this.setValue(val);
    /** OBSOLET **
    val = this._roundToGrid(val);
    this.updateInputBox(val);
    if ('undefined' != typeof(this.eventOnChange)) this.eventOnChange(this, val);
    this.fireEvent('onChange');
   */
  }
  
  
  /**
  * DEPRECATED
  * updates the input field box and/or the text with the value.
  * @param  string val (number)
  * @access private
  * @return void
  * @deprecated
  */
  this.updateInputBox = function(val) {
    this.setValue(val);
    /** OBSOLET **
    val = this._roundToGrid(val);
    if ('undefined' != typeof(this.localInput)) {
      this.localInput.value = val;    
    }
    if ('undefined' != typeof(this.localText)) {
      this.localText.innerHTML = val; //innerText
    }
    */
  }
  
  
  /**
  * @access private
  * @param bool editMode
  */
  this.textboxEdit = function(editMode) {
    if (this._disabled) return;
    if (editMode) {
      if ('undefined' != typeof(this._valueFieldObj)) {
        this._valueTextObj.style.display = 'none';
        this._valueFieldObj.style.display = 'block';
        bsFormFieldSetFocusAndSelect(this._valueFieldObj, false);
      }
    } else {
      if ('undefined' != typeof(this._valueTextObj)) {
        this._valueFieldObj.style.display = 'none';
        this._valueTextObj.style.display  = 'block';
      }
    }
  }
  
  /**
  * Fires during the mause move off the slider-handle (as long as the mouse button is hold)
  * @access private
  * @param  object ctrl The slider handle object
  * @param  object client
  */
  this.slideMove = function(ctrl, client) {
    ctrl.sliderObj.onChangeBySlide(ctrl);
  }


  /**
  * Fires on first click off the slider-handle
  * @access private
  * @param  object ctrl The slider handle object
  * @param  ? client
  */
  this.slideStart = function(ctrl,client) {
    if (ctrl.sliderObj._disabled) return;
    ctrl.sliderObj._posEventSlideStartX = ctrl.startX;
    ctrl.sliderObj._posEventSlideStartY = ctrl.startY;
    ctrl.sliderObj._posObjSlideStartX = ctrl.sliderObj._handleObj.style.left;
    ctrl.sliderObj._posObjSlideStartY = ctrl.sliderObj._handleObj.style.top;
    
    var pos = ctrl.sliderObj.getSliderPos();
    ctrl.sliderObj.setValue(pos);
    
    if ('undefined' != typeof(ctrl.sliderObj.slideStartCB)) {
      ctrl.sliderObj.slideStartCB(ctrl.sliderObj, ctrl.sliderObj.getValue(), pos);
    }
  }
  
  
  /**
  * Fires on first unclick off the slider-handle
  * @access private
  * @param  object ctrl The slider handle object
  * @param  ? client
  */
  this.slideEnd = function(ctrl,client){
    var pos = ctrl.sliderObj.getSliderPos();
    if ('undefined' != typeof(ctrl.sliderObj.slideEndCB)) {
      ctrl.sliderObj.slideEndCB(ctrl.sliderObj, ctrl.sliderObj.getValue(), pos);
    }

    return;
  }
  
  ///////////////////////////////////////////
  // constructor code
  ///////////////////////////////////////////
	this._constructor(theFieldnamePrefix); //call the constructor. needs to be at the end.
 
}

