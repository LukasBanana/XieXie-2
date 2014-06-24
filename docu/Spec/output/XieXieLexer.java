// $ANTLR 3.5 D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g 2014-06-10 20:58:56

import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

@SuppressWarnings("all")
public class XieXieLexer extends Lexer {
	public static final int EOF=-1;
	public static final int T__28=28;
	public static final int T__29=29;
	public static final int T__30=30;
	public static final int T__31=31;
	public static final int T__32=32;
	public static final int T__33=33;
	public static final int T__34=34;
	public static final int T__35=35;
	public static final int T__36=36;
	public static final int T__37=37;
	public static final int T__38=38;
	public static final int T__39=39;
	public static final int T__40=40;
	public static final int T__41=41;
	public static final int T__42=42;
	public static final int T__43=43;
	public static final int T__44=44;
	public static final int T__45=45;
	public static final int T__46=46;
	public static final int T__47=47;
	public static final int T__48=48;
	public static final int T__49=49;
	public static final int T__50=50;
	public static final int T__51=51;
	public static final int T__52=52;
	public static final int T__53=53;
	public static final int T__54=54;
	public static final int T__55=55;
	public static final int T__56=56;
	public static final int T__57=57;
	public static final int T__58=58;
	public static final int T__59=59;
	public static final int T__60=60;
	public static final int T__61=61;
	public static final int T__62=62;
	public static final int T__63=63;
	public static final int T__64=64;
	public static final int T__65=65;
	public static final int T__66=66;
	public static final int T__67=67;
	public static final int T__68=68;
	public static final int T__69=69;
	public static final int T__70=70;
	public static final int T__71=71;
	public static final int T__72=72;
	public static final int T__73=73;
	public static final int T__74=74;
	public static final int T__75=75;
	public static final int T__76=76;
	public static final int T__77=77;
	public static final int T__78=78;
	public static final int T__79=79;
	public static final int T__80=80;
	public static final int T__81=81;
	public static final int T__82=82;
	public static final int T__83=83;
	public static final int T__84=84;
	public static final int T__85=85;
	public static final int T__86=86;
	public static final int T__87=87;
	public static final int T__88=88;
	public static final int T__89=89;
	public static final int T__90=90;
	public static final int BIN_LITERAL=4;
	public static final int BLOCK_COMMENT=5;
	public static final int BOOL_LITERAL=6;
	public static final int BOOL_TYPE_DENOTER=7;
	public static final int BinDigit=8;
	public static final int CHAR_LITERAL=9;
	public static final int DEC_LITERAL=10;
	public static final int DecDigit=11;
	public static final int EscapeCharacter=12;
	public static final int FLOAT_LITERAL=13;
	public static final int FLOAT_TYPE_DENOTER=14;
	public static final int HEX_LITERAL=15;
	public static final int HexDigit=16;
	public static final int IDENT=17;
	public static final int INT_LITERAL=18;
	public static final int INT_TYPE_DENOTER=19;
	public static final int LINE_COMMENT=20;
	public static final int Letter=21;
	public static final int NULL_LITERAL=22;
	public static final int OCT_LITERAL=23;
	public static final int OctDigit=24;
	public static final int SUPER_LITERAL=25;
	public static final int THIS_LITERAL=26;
	public static final int WS=27;

	// delegates
	// delegators
	public Lexer[] getDelegates() {
		return new Lexer[] {};
	}

	public XieXieLexer() {} 
	public XieXieLexer(CharStream input) {
		this(input, new RecognizerSharedState());
	}
	public XieXieLexer(CharStream input, RecognizerSharedState state) {
		super(input,state);
	}
	@Override public String getGrammarFileName() { return "D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g"; }

	// $ANTLR start "T__28"
	public final void mT__28() throws RecognitionException {
		try {
			int _type = T__28;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:2:7: ( '!=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:2:9: '!='
			{
			match("!="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__28"

	// $ANTLR start "T__29"
	public final void mT__29() throws RecognitionException {
		try {
			int _type = T__29;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:3:7: ( '%' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:3:9: '%'
			{
			match('%'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__29"

	// $ANTLR start "T__30"
	public final void mT__30() throws RecognitionException {
		try {
			int _type = T__30;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:4:7: ( '%=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:4:9: '%='
			{
			match("%="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__30"

	// $ANTLR start "T__31"
	public final void mT__31() throws RecognitionException {
		try {
			int _type = T__31;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:5:7: ( '&' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:5:9: '&'
			{
			match('&'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__31"

	// $ANTLR start "T__32"
	public final void mT__32() throws RecognitionException {
		try {
			int _type = T__32;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:6:7: ( '&=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:6:9: '&='
			{
			match("&="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__32"

	// $ANTLR start "T__33"
	public final void mT__33() throws RecognitionException {
		try {
			int _type = T__33;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:7:7: ( '(' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:7:9: '('
			{
			match('('); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__33"

	// $ANTLR start "T__34"
	public final void mT__34() throws RecognitionException {
		try {
			int _type = T__34;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:8:7: ( ')' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:8:9: ')'
			{
			match(')'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__34"

	// $ANTLR start "T__35"
	public final void mT__35() throws RecognitionException {
		try {
			int _type = T__35;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:9:7: ( '*' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:9:9: '*'
			{
			match('*'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__35"

	// $ANTLR start "T__36"
	public final void mT__36() throws RecognitionException {
		try {
			int _type = T__36;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:10:7: ( '*=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:10:9: '*='
			{
			match("*="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__36"

	// $ANTLR start "T__37"
	public final void mT__37() throws RecognitionException {
		try {
			int _type = T__37;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:11:7: ( '+' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:11:9: '+'
			{
			match('+'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__37"

	// $ANTLR start "T__38"
	public final void mT__38() throws RecognitionException {
		try {
			int _type = T__38;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:12:7: ( '++' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:12:9: '++'
			{
			match("++"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__38"

	// $ANTLR start "T__39"
	public final void mT__39() throws RecognitionException {
		try {
			int _type = T__39;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:13:7: ( '+=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:13:9: '+='
			{
			match("+="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__39"

	// $ANTLR start "T__40"
	public final void mT__40() throws RecognitionException {
		try {
			int _type = T__40;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:7: ( ',' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:14:9: ','
			{
			match(','); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__40"

	// $ANTLR start "T__41"
	public final void mT__41() throws RecognitionException {
		try {
			int _type = T__41;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:15:7: ( '-' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:15:9: '-'
			{
			match('-'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__41"

	// $ANTLR start "T__42"
	public final void mT__42() throws RecognitionException {
		try {
			int _type = T__42;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:7: ( '--' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:16:9: '--'
			{
			match("--"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__42"

	// $ANTLR start "T__43"
	public final void mT__43() throws RecognitionException {
		try {
			int _type = T__43;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:17:7: ( '-=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:17:9: '-='
			{
			match("-="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__43"

	// $ANTLR start "T__44"
	public final void mT__44() throws RecognitionException {
		try {
			int _type = T__44;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:18:7: ( '->' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:18:9: '->'
			{
			match("->"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__44"

	// $ANTLR start "T__45"
	public final void mT__45() throws RecognitionException {
		try {
			int _type = T__45;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:7: ( '.' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:19:9: '.'
			{
			match('.'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__45"

	// $ANTLR start "T__46"
	public final void mT__46() throws RecognitionException {
		try {
			int _type = T__46;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:7: ( '..' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:20:9: '..'
			{
			match(".."); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__46"

	// $ANTLR start "T__47"
	public final void mT__47() throws RecognitionException {
		try {
			int _type = T__47;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:7: ( '/' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:21:9: '/'
			{
			match('/'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__47"

	// $ANTLR start "T__48"
	public final void mT__48() throws RecognitionException {
		try {
			int _type = T__48;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:22:7: ( '/=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:22:9: '/='
			{
			match("/="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__48"

	// $ANTLR start "T__49"
	public final void mT__49() throws RecognitionException {
		try {
			int _type = T__49;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:7: ( ':' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:23:9: ':'
			{
			match(':'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__49"

	// $ANTLR start "T__50"
	public final void mT__50() throws RecognitionException {
		try {
			int _type = T__50;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:24:7: ( ':=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:24:9: ':='
			{
			match(":="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__50"

	// $ANTLR start "T__51"
	public final void mT__51() throws RecognitionException {
		try {
			int _type = T__51;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:25:7: ( ';' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:25:9: ';'
			{
			match(';'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__51"

	// $ANTLR start "T__52"
	public final void mT__52() throws RecognitionException {
		try {
			int _type = T__52;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:26:7: ( '<' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:26:9: '<'
			{
			match('<'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__52"

	// $ANTLR start "T__53"
	public final void mT__53() throws RecognitionException {
		try {
			int _type = T__53;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:27:7: ( '<<' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:27:9: '<<'
			{
			match("<<"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__53"

	// $ANTLR start "T__54"
	public final void mT__54() throws RecognitionException {
		try {
			int _type = T__54;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:28:7: ( '<<=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:28:9: '<<='
			{
			match("<<="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__54"

	// $ANTLR start "T__55"
	public final void mT__55() throws RecognitionException {
		try {
			int _type = T__55;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:7: ( '<=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:29:9: '<='
			{
			match("<="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__55"

	// $ANTLR start "T__56"
	public final void mT__56() throws RecognitionException {
		try {
			int _type = T__56;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:30:7: ( '=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:30:9: '='
			{
			match('='); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__56"

	// $ANTLR start "T__57"
	public final void mT__57() throws RecognitionException {
		try {
			int _type = T__57;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:31:7: ( '>' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:31:9: '>'
			{
			match('>'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__57"

	// $ANTLR start "T__58"
	public final void mT__58() throws RecognitionException {
		try {
			int _type = T__58;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:32:7: ( '>=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:32:9: '>='
			{
			match(">="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__58"

	// $ANTLR start "T__59"
	public final void mT__59() throws RecognitionException {
		try {
			int _type = T__59;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:33:7: ( '>>' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:33:9: '>>'
			{
			match(">>"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__59"

	// $ANTLR start "T__60"
	public final void mT__60() throws RecognitionException {
		try {
			int _type = T__60;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:34:7: ( '>>=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:34:9: '>>='
			{
			match(">>="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__60"

	// $ANTLR start "T__61"
	public final void mT__61() throws RecognitionException {
		try {
			int _type = T__61;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:35:7: ( '[[' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:35:9: '[['
			{
			match("[["); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__61"

	// $ANTLR start "T__62"
	public final void mT__62() throws RecognitionException {
		try {
			int _type = T__62;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:36:7: ( ']]' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:36:9: ']]'
			{
			match("]]"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__62"

	// $ANTLR start "T__63"
	public final void mT__63() throws RecognitionException {
		try {
			int _type = T__63;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:37:7: ( '^' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:37:9: '^'
			{
			match('^'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__63"

	// $ANTLR start "T__64"
	public final void mT__64() throws RecognitionException {
		try {
			int _type = T__64;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:7: ( '^=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:38:9: '^='
			{
			match("^="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__64"

	// $ANTLR start "T__65"
	public final void mT__65() throws RecognitionException {
		try {
			int _type = T__65;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:39:7: ( 'and' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:39:9: 'and'
			{
			match("and"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__65"

	// $ANTLR start "T__66"
	public final void mT__66() throws RecognitionException {
		try {
			int _type = T__66;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:40:7: ( 'break' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:40:9: 'break'
			{
			match("break"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__66"

	// $ANTLR start "T__67"
	public final void mT__67() throws RecognitionException {
		try {
			int _type = T__67;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:41:7: ( 'case' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:41:9: 'case'
			{
			match("case"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__67"

	// $ANTLR start "T__68"
	public final void mT__68() throws RecognitionException {
		try {
			int _type = T__68;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:42:7: ( 'class' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:42:9: 'class'
			{
			match("class"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__68"

	// $ANTLR start "T__69"
	public final void mT__69() throws RecognitionException {
		try {
			int _type = T__69;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:43:7: ( 'continue' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:43:9: 'continue'
			{
			match("continue"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__69"

	// $ANTLR start "T__70"
	public final void mT__70() throws RecognitionException {
		try {
			int _type = T__70;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:44:7: ( 'default' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:44:9: 'default'
			{
			match("default"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__70"

	// $ANTLR start "T__71"
	public final void mT__71() throws RecognitionException {
		try {
			int _type = T__71;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:45:7: ( 'do' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:45:9: 'do'
			{
			match("do"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__71"

	// $ANTLR start "T__72"
	public final void mT__72() throws RecognitionException {
		try {
			int _type = T__72;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:46:7: ( 'else' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:46:9: 'else'
			{
			match("else"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__72"

	// $ANTLR start "T__73"
	public final void mT__73() throws RecognitionException {
		try {
			int _type = T__73;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:47:7: ( 'enum' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:47:9: 'enum'
			{
			match("enum"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__73"

	// $ANTLR start "T__74"
	public final void mT__74() throws RecognitionException {
		try {
			int _type = T__74;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:48:7: ( 'extern' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:48:9: 'extern'
			{
			match("extern"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__74"

	// $ANTLR start "T__75"
	public final void mT__75() throws RecognitionException {
		try {
			int _type = T__75;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:7: ( 'flags' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:49:9: 'flags'
			{
			match("flags"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__75"

	// $ANTLR start "T__76"
	public final void mT__76() throws RecognitionException {
		try {
			int _type = T__76;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:50:7: ( 'for' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:50:9: 'for'
			{
			match("for"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__76"

	// $ANTLR start "T__77"
	public final void mT__77() throws RecognitionException {
		try {
			int _type = T__77;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:51:7: ( 'foreach' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:51:9: 'foreach'
			{
			match("foreach"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__77"

	// $ANTLR start "T__78"
	public final void mT__78() throws RecognitionException {
		try {
			int _type = T__78;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:52:7: ( 'forever' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:52:9: 'forever'
			{
			match("forever"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__78"

	// $ANTLR start "T__79"
	public final void mT__79() throws RecognitionException {
		try {
			int _type = T__79;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:53:7: ( 'if' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:53:9: 'if'
			{
			match("if"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__79"

	// $ANTLR start "T__80"
	public final void mT__80() throws RecognitionException {
		try {
			int _type = T__80;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:54:7: ( 'not' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:54:9: 'not'
			{
			match("not"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__80"

	// $ANTLR start "T__81"
	public final void mT__81() throws RecognitionException {
		try {
			int _type = T__81;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:55:7: ( 'or' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:55:9: 'or'
			{
			match("or"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__81"

	// $ANTLR start "T__82"
	public final void mT__82() throws RecognitionException {
		try {
			int _type = T__82;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:56:7: ( 'return' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:56:9: 'return'
			{
			match("return"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__82"

	// $ANTLR start "T__83"
	public final void mT__83() throws RecognitionException {
		try {
			int _type = T__83;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:57:7: ( 'static' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:57:9: 'static'
			{
			match("static"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__83"

	// $ANTLR start "T__84"
	public final void mT__84() throws RecognitionException {
		try {
			int _type = T__84;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:58:7: ( 'switch' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:58:9: 'switch'
			{
			match("switch"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__84"

	// $ANTLR start "T__85"
	public final void mT__85() throws RecognitionException {
		try {
			int _type = T__85;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:7: ( 'while' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:59:9: 'while'
			{
			match("while"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__85"

	// $ANTLR start "T__86"
	public final void mT__86() throws RecognitionException {
		try {
			int _type = T__86;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:60:7: ( '{' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:60:9: '{'
			{
			match('{'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__86"

	// $ANTLR start "T__87"
	public final void mT__87() throws RecognitionException {
		try {
			int _type = T__87;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:7: ( '|' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:61:9: '|'
			{
			match('|'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__87"

	// $ANTLR start "T__88"
	public final void mT__88() throws RecognitionException {
		try {
			int _type = T__88;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:62:7: ( '|=' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:62:9: '|='
			{
			match("|="); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__88"

	// $ANTLR start "T__89"
	public final void mT__89() throws RecognitionException {
		try {
			int _type = T__89;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:7: ( '}' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:63:9: '}'
			{
			match('}'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__89"

	// $ANTLR start "T__90"
	public final void mT__90() throws RecognitionException {
		try {
			int _type = T__90;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:7: ( '~' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:64:9: '~'
			{
			match('~'); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "T__90"

	// $ANTLR start "BOOL_TYPE_DENOTER"
	public final void mBOOL_TYPE_DENOTER() throws RecognitionException {
		try {
			int _type = BOOL_TYPE_DENOTER;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:182:18: ( 'bool' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:182:21: 'bool'
			{
			match("bool"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "BOOL_TYPE_DENOTER"

	// $ANTLR start "INT_TYPE_DENOTER"
	public final void mINT_TYPE_DENOTER() throws RecognitionException {
		try {
			int _type = INT_TYPE_DENOTER;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:183:17: ( 'int' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:183:20: 'int'
			{
			match("int"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "INT_TYPE_DENOTER"

	// $ANTLR start "FLOAT_TYPE_DENOTER"
	public final void mFLOAT_TYPE_DENOTER() throws RecognitionException {
		try {
			int _type = FLOAT_TYPE_DENOTER;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:184:19: ( 'float' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:184:22: 'float'
			{
			match("float"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "FLOAT_TYPE_DENOTER"

	// $ANTLR start "Letter"
	public final void mLetter() throws RecognitionException {
		try {
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:189:7: ( ( 'a' .. 'z' | 'A' .. 'Z' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
			{
			if ( (input.LA(1) >= 'A' && input.LA(1) <= 'Z')||(input.LA(1) >= 'a' && input.LA(1) <= 'z') ) {
				input.consume();
			}
			else {
				MismatchedSetException mse = new MismatchedSetException(null,input);
				recover(mse);
				throw mse;
			}
			}

		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "Letter"

	// $ANTLR start "BinDigit"
	public final void mBinDigit() throws RecognitionException {
		try {
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:192:9: ( ( '0' .. '1' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
			{
			if ( (input.LA(1) >= '0' && input.LA(1) <= '1') ) {
				input.consume();
			}
			else {
				MismatchedSetException mse = new MismatchedSetException(null,input);
				recover(mse);
				throw mse;
			}
			}

		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "BinDigit"

	// $ANTLR start "OctDigit"
	public final void mOctDigit() throws RecognitionException {
		try {
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:195:9: ( ( '0' .. '7' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
			{
			if ( (input.LA(1) >= '0' && input.LA(1) <= '7') ) {
				input.consume();
			}
			else {
				MismatchedSetException mse = new MismatchedSetException(null,input);
				recover(mse);
				throw mse;
			}
			}

		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "OctDigit"

	// $ANTLR start "DecDigit"
	public final void mDecDigit() throws RecognitionException {
		try {
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:198:9: ( ( '0' .. '9' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
			{
			if ( (input.LA(1) >= '0' && input.LA(1) <= '9') ) {
				input.consume();
			}
			else {
				MismatchedSetException mse = new MismatchedSetException(null,input);
				recover(mse);
				throw mse;
			}
			}

		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "DecDigit"

	// $ANTLR start "HexDigit"
	public final void mHexDigit() throws RecognitionException {
		try {
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:201:9: ( ( '0' .. '9' | 'a' .. 'f' | 'A' .. 'F' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
			{
			if ( (input.LA(1) >= '0' && input.LA(1) <= '9')||(input.LA(1) >= 'A' && input.LA(1) <= 'F')||(input.LA(1) >= 'a' && input.LA(1) <= 'f') ) {
				input.consume();
			}
			else {
				MismatchedSetException mse = new MismatchedSetException(null,input);
				recover(mse);
				throw mse;
			}
			}

		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "HexDigit"

	// $ANTLR start "BIN_LITERAL"
	public final void mBIN_LITERAL() throws RecognitionException {
		try {
			int _type = BIN_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:210:12: ( '0b' ( BinDigit )+ )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:210:14: '0b' ( BinDigit )+
			{
			match("0b"); 

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:210:19: ( BinDigit )+
			int cnt1=0;
			loop1:
			while (true) {
				int alt1=2;
				int LA1_0 = input.LA(1);
				if ( ((LA1_0 >= '0' && LA1_0 <= '1')) ) {
					alt1=1;
				}

				switch (alt1) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
					{
					if ( (input.LA(1) >= '0' && input.LA(1) <= '1') ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;

				default :
					if ( cnt1 >= 1 ) break loop1;
					EarlyExitException eee = new EarlyExitException(1, input);
					throw eee;
				}
				cnt1++;
			}

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "BIN_LITERAL"

	// $ANTLR start "OCT_LITERAL"
	public final void mOCT_LITERAL() throws RecognitionException {
		try {
			int _type = OCT_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:211:12: ( '0o' ( OctDigit )+ )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:211:14: '0o' ( OctDigit )+
			{
			match("0o"); 

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:211:19: ( OctDigit )+
			int cnt2=0;
			loop2:
			while (true) {
				int alt2=2;
				int LA2_0 = input.LA(1);
				if ( ((LA2_0 >= '0' && LA2_0 <= '7')) ) {
					alt2=1;
				}

				switch (alt2) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
					{
					if ( (input.LA(1) >= '0' && input.LA(1) <= '7') ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;

				default :
					if ( cnt2 >= 1 ) break loop2;
					EarlyExitException eee = new EarlyExitException(2, input);
					throw eee;
				}
				cnt2++;
			}

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "OCT_LITERAL"

	// $ANTLR start "DEC_LITERAL"
	public final void mDEC_LITERAL() throws RecognitionException {
		try {
			int _type = DEC_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:212:12: ( ( '0' | '1' .. '9' ( DecDigit )* ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:212:14: ( '0' | '1' .. '9' ( DecDigit )* )
			{
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:212:14: ( '0' | '1' .. '9' ( DecDigit )* )
			int alt4=2;
			int LA4_0 = input.LA(1);
			if ( (LA4_0=='0') ) {
				alt4=1;
			}
			else if ( ((LA4_0 >= '1' && LA4_0 <= '9')) ) {
				alt4=2;
			}

			else {
				NoViableAltException nvae =
					new NoViableAltException("", 4, 0, input);
				throw nvae;
			}

			switch (alt4) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:212:16: '0'
					{
					match('0'); 
					}
					break;
				case 2 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:212:22: '1' .. '9' ( DecDigit )*
					{
					matchRange('1','9'); 
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:212:31: ( DecDigit )*
					loop3:
					while (true) {
						int alt3=2;
						int LA3_0 = input.LA(1);
						if ( ((LA3_0 >= '0' && LA3_0 <= '9')) ) {
							alt3=1;
						}

						switch (alt3) {
						case 1 :
							// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
							{
							if ( (input.LA(1) >= '0' && input.LA(1) <= '9') ) {
								input.consume();
							}
							else {
								MismatchedSetException mse = new MismatchedSetException(null,input);
								recover(mse);
								throw mse;
							}
							}
							break;

						default :
							break loop3;
						}
					}

					}
					break;

			}

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "DEC_LITERAL"

	// $ANTLR start "HEX_LITERAL"
	public final void mHEX_LITERAL() throws RecognitionException {
		try {
			int _type = HEX_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:213:12: ( '0x' ( HexDigit )+ )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:213:14: '0x' ( HexDigit )+
			{
			match("0x"); 

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:213:19: ( HexDigit )+
			int cnt5=0;
			loop5:
			while (true) {
				int alt5=2;
				int LA5_0 = input.LA(1);
				if ( ((LA5_0 >= '0' && LA5_0 <= '9')||(LA5_0 >= 'A' && LA5_0 <= 'F')||(LA5_0 >= 'a' && LA5_0 <= 'f')) ) {
					alt5=1;
				}

				switch (alt5) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
					{
					if ( (input.LA(1) >= '0' && input.LA(1) <= '9')||(input.LA(1) >= 'A' && input.LA(1) <= 'F')||(input.LA(1) >= 'a' && input.LA(1) <= 'f') ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;

				default :
					if ( cnt5 >= 1 ) break loop5;
					EarlyExitException eee = new EarlyExitException(5, input);
					throw eee;
				}
				cnt5++;
			}

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "HEX_LITERAL"

	// $ANTLR start "NULL_LITERAL"
	public final void mNULL_LITERAL() throws RecognitionException {
		try {
			int _type = NULL_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:215:13: ( 'null' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:215:15: 'null'
			{
			match("null"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "NULL_LITERAL"

	// $ANTLR start "THIS_LITERAL"
	public final void mTHIS_LITERAL() throws RecognitionException {
		try {
			int _type = THIS_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:216:13: ( 'this' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:216:15: 'this'
			{
			match("this"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "THIS_LITERAL"

	// $ANTLR start "SUPER_LITERAL"
	public final void mSUPER_LITERAL() throws RecognitionException {
		try {
			int _type = SUPER_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:217:14: ( 'super' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:217:16: 'super'
			{
			match("super"); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "SUPER_LITERAL"

	// $ANTLR start "BOOL_LITERAL"
	public final void mBOOL_LITERAL() throws RecognitionException {
		try {
			int _type = BOOL_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:219:13: ( ( 'true' | 'false' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:219:15: ( 'true' | 'false' )
			{
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:219:15: ( 'true' | 'false' )
			int alt6=2;
			int LA6_0 = input.LA(1);
			if ( (LA6_0=='t') ) {
				alt6=1;
			}
			else if ( (LA6_0=='f') ) {
				alt6=2;
			}

			else {
				NoViableAltException nvae =
					new NoViableAltException("", 6, 0, input);
				throw nvae;
			}

			switch (alt6) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:219:17: 'true'
					{
					match("true"); 

					}
					break;
				case 2 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:219:26: 'false'
					{
					match("false"); 

					}
					break;

			}

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "BOOL_LITERAL"

	// $ANTLR start "INT_LITERAL"
	public final void mINT_LITERAL() throws RecognitionException {
		try {
			int _type = INT_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:221:13: (| DEC_LITERAL )
			int alt7=2;
			int LA7_0 = input.LA(1);
			if ( ((LA7_0 >= '0' && LA7_0 <= '9')) ) {
				alt7=2;
			}

			else {
				alt7=1;
			}

			switch (alt7) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:223:4: 
					{
					}
					break;
				case 2 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:223:6: DEC_LITERAL
					{
					mDEC_LITERAL(); 

					}
					break;

			}
			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "INT_LITERAL"

	// $ANTLR start "FLOAT_LITERAL"
	public final void mFLOAT_LITERAL() throws RecognitionException {
		try {
			int _type = FLOAT_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:226:14: ( DEC_LITERAL '.' DEC_LITERAL )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:226:16: DEC_LITERAL '.' DEC_LITERAL
			{
			mDEC_LITERAL(); 

			match('.'); 
			mDEC_LITERAL(); 

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "FLOAT_LITERAL"

	// $ANTLR start "CHAR_LITERAL"
	public final void mCHAR_LITERAL() throws RecognitionException {
		try {
			int _type = CHAR_LITERAL;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:227:13: ( '\\'' ( EscapeCharacter |~ ( '\\'' | '\\\\' | '\\r' | '\\n' ) ) '\\'' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:227:15: '\\'' ( EscapeCharacter |~ ( '\\'' | '\\\\' | '\\r' | '\\n' ) ) '\\''
			{
			match('\''); 
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:227:20: ( EscapeCharacter |~ ( '\\'' | '\\\\' | '\\r' | '\\n' ) )
			int alt8=2;
			int LA8_0 = input.LA(1);
			if ( (LA8_0=='\\') ) {
				alt8=1;
			}
			else if ( ((LA8_0 >= '\u0000' && LA8_0 <= '\t')||(LA8_0 >= '\u000B' && LA8_0 <= '\f')||(LA8_0 >= '\u000E' && LA8_0 <= '&')||(LA8_0 >= '(' && LA8_0 <= '[')||(LA8_0 >= ']' && LA8_0 <= '\uFFFF')) ) {
				alt8=2;
			}

			else {
				NoViableAltException nvae =
					new NoViableAltException("", 8, 0, input);
				throw nvae;
			}

			switch (alt8) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:227:22: EscapeCharacter
					{
					mEscapeCharacter(); 

					}
					break;
				case 2 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:227:40: ~ ( '\\'' | '\\\\' | '\\r' | '\\n' )
					{
					if ( (input.LA(1) >= '\u0000' && input.LA(1) <= '\t')||(input.LA(1) >= '\u000B' && input.LA(1) <= '\f')||(input.LA(1) >= '\u000E' && input.LA(1) <= '&')||(input.LA(1) >= '(' && input.LA(1) <= '[')||(input.LA(1) >= ']' && input.LA(1) <= '\uFFFF') ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;

			}

			match('\''); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "CHAR_LITERAL"

	// $ANTLR start "EscapeCharacter"
	public final void mEscapeCharacter() throws RecognitionException {
		try {
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:239:17: ( '\\\\' ( 'b' | 't' | 'n' | 'f' | 'r' | '\\\"' | '\\'' | '\\\\' ) | '\\\\x' HexDigit HexDigit | '\\\\u' HexDigit HexDigit HexDigit HexDigit | '\\\\U' HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit )
			int alt9=4;
			int LA9_0 = input.LA(1);
			if ( (LA9_0=='\\') ) {
				switch ( input.LA(2) ) {
				case '\"':
				case '\'':
				case '\\':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					{
					alt9=1;
					}
					break;
				case 'x':
					{
					alt9=2;
					}
					break;
				case 'u':
					{
					alt9=3;
					}
					break;
				case 'U':
					{
					alt9=4;
					}
					break;
				default:
					int nvaeMark = input.mark();
					try {
						input.consume();
						NoViableAltException nvae =
							new NoViableAltException("", 9, 1, input);
						throw nvae;
					} finally {
						input.rewind(nvaeMark);
					}
				}
			}

			else {
				NoViableAltException nvae =
					new NoViableAltException("", 9, 0, input);
				throw nvae;
			}

			switch (alt9) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:239:19: '\\\\' ( 'b' | 't' | 'n' | 'f' | 'r' | '\\\"' | '\\'' | '\\\\' )
					{
					match('\\'); 
					if ( input.LA(1)=='\"'||input.LA(1)=='\''||input.LA(1)=='\\'||input.LA(1)=='b'||input.LA(1)=='f'||input.LA(1)=='n'||input.LA(1)=='r'||input.LA(1)=='t' ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;
				case 2 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:240:7: '\\\\x' HexDigit HexDigit
					{
					match("\\x"); 

					mHexDigit(); 

					mHexDigit(); 

					}
					break;
				case 3 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:241:7: '\\\\u' HexDigit HexDigit HexDigit HexDigit
					{
					match("\\u"); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					}
					break;
				case 4 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:242:7: '\\\\U' HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit
					{
					match("\\U"); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					mHexDigit(); 

					}
					break;

			}
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "EscapeCharacter"

	// $ANTLR start "IDENT"
	public final void mIDENT() throws RecognitionException {
		try {
			int _type = IDENT;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:243:6: ( Letter ( Letter | '0' .. '9' | '_' )* )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:243:8: Letter ( Letter | '0' .. '9' | '_' )*
			{
			mLetter(); 

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:243:15: ( Letter | '0' .. '9' | '_' )*
			loop10:
			while (true) {
				int alt10=2;
				int LA10_0 = input.LA(1);
				if ( ((LA10_0 >= '0' && LA10_0 <= '9')||(LA10_0 >= 'A' && LA10_0 <= 'Z')||LA10_0=='_'||(LA10_0 >= 'a' && LA10_0 <= 'z')) ) {
					alt10=1;
				}

				switch (alt10) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
					{
					if ( (input.LA(1) >= '0' && input.LA(1) <= '9')||(input.LA(1) >= 'A' && input.LA(1) <= 'Z')||input.LA(1)=='_'||(input.LA(1) >= 'a' && input.LA(1) <= 'z') ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;

				default :
					break loop10;
				}
			}

			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "IDENT"

	// $ANTLR start "WS"
	public final void mWS() throws RecognitionException {
		try {
			int _type = WS;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:246:3: ( ( ' ' | '\\r' | '\\t' | '\\u000C' | '\\n' ) )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:246:6: ( ' ' | '\\r' | '\\t' | '\\u000C' | '\\n' )
			{
			if ( (input.LA(1) >= '\t' && input.LA(1) <= '\n')||(input.LA(1) >= '\f' && input.LA(1) <= '\r')||input.LA(1)==' ' ) {
				input.consume();
			}
			else {
				MismatchedSetException mse = new MismatchedSetException(null,input);
				recover(mse);
				throw mse;
			}
			 skip(); 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "WS"

	// $ANTLR start "BLOCK_COMMENT"
	public final void mBLOCK_COMMENT() throws RecognitionException {
		try {
			int _type = BLOCK_COMMENT;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:248:14: ( '/*' ( options {greedy=false; } : . )* '*/' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:248:16: '/*' ( options {greedy=false; } : . )* '*/'
			{
			match("/*"); 

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:248:21: ( options {greedy=false; } : . )*
			loop11:
			while (true) {
				int alt11=2;
				int LA11_0 = input.LA(1);
				if ( (LA11_0=='*') ) {
					int LA11_1 = input.LA(2);
					if ( (LA11_1=='/') ) {
						alt11=2;
					}
					else if ( ((LA11_1 >= '\u0000' && LA11_1 <= '.')||(LA11_1 >= '0' && LA11_1 <= '\uFFFF')) ) {
						alt11=1;
					}

				}
				else if ( ((LA11_0 >= '\u0000' && LA11_0 <= ')')||(LA11_0 >= '+' && LA11_0 <= '\uFFFF')) ) {
					alt11=1;
				}

				switch (alt11) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:248:53: .
					{
					matchAny(); 
					}
					break;

				default :
					break loop11;
				}
			}

			match("*/"); 

			 _channel = HIDDEN; 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "BLOCK_COMMENT"

	// $ANTLR start "LINE_COMMENT"
	public final void mLINE_COMMENT() throws RecognitionException {
		try {
			int _type = LINE_COMMENT;
			int _channel = DEFAULT_TOKEN_CHANNEL;
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:249:13: ( '//' (~ ( '\\n' | '\\r' ) )* ( '\\r' )? '\\n' )
			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:249:15: '//' (~ ( '\\n' | '\\r' ) )* ( '\\r' )? '\\n'
			{
			match("//"); 

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:249:20: (~ ( '\\n' | '\\r' ) )*
			loop12:
			while (true) {
				int alt12=2;
				int LA12_0 = input.LA(1);
				if ( ((LA12_0 >= '\u0000' && LA12_0 <= '\t')||(LA12_0 >= '\u000B' && LA12_0 <= '\f')||(LA12_0 >= '\u000E' && LA12_0 <= '\uFFFF')) ) {
					alt12=1;
				}

				switch (alt12) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:
					{
					if ( (input.LA(1) >= '\u0000' && input.LA(1) <= '\t')||(input.LA(1) >= '\u000B' && input.LA(1) <= '\f')||(input.LA(1) >= '\u000E' && input.LA(1) <= '\uFFFF') ) {
						input.consume();
					}
					else {
						MismatchedSetException mse = new MismatchedSetException(null,input);
						recover(mse);
						throw mse;
					}
					}
					break;

				default :
					break loop12;
				}
			}

			// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:249:34: ( '\\r' )?
			int alt13=2;
			int LA13_0 = input.LA(1);
			if ( (LA13_0=='\r') ) {
				alt13=1;
			}
			switch (alt13) {
				case 1 :
					// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:249:34: '\\r'
					{
					match('\r'); 
					}
					break;

			}

			match('\n'); 
			 _channel = HIDDEN; 
			}

			state.type = _type;
			state.channel = _channel;
		}
		finally {
			// do for sure before leaving
		}
	}
	// $ANTLR end "LINE_COMMENT"

	@Override
	public void mTokens() throws RecognitionException {
		// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:8: ( T__28 | T__29 | T__30 | T__31 | T__32 | T__33 | T__34 | T__35 | T__36 | T__37 | T__38 | T__39 | T__40 | T__41 | T__42 | T__43 | T__44 | T__45 | T__46 | T__47 | T__48 | T__49 | T__50 | T__51 | T__52 | T__53 | T__54 | T__55 | T__56 | T__57 | T__58 | T__59 | T__60 | T__61 | T__62 | T__63 | T__64 | T__65 | T__66 | T__67 | T__68 | T__69 | T__70 | T__71 | T__72 | T__73 | T__74 | T__75 | T__76 | T__77 | T__78 | T__79 | T__80 | T__81 | T__82 | T__83 | T__84 | T__85 | T__86 | T__87 | T__88 | T__89 | T__90 | BOOL_TYPE_DENOTER | INT_TYPE_DENOTER | FLOAT_TYPE_DENOTER | BIN_LITERAL | OCT_LITERAL | DEC_LITERAL | HEX_LITERAL | NULL_LITERAL | THIS_LITERAL | SUPER_LITERAL | BOOL_LITERAL | INT_LITERAL | FLOAT_LITERAL | CHAR_LITERAL | IDENT | WS | BLOCK_COMMENT | LINE_COMMENT )
		int alt14=81;
		alt14 = dfa14.predict(input);
		switch (alt14) {
			case 1 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:10: T__28
				{
				mT__28(); 

				}
				break;
			case 2 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:16: T__29
				{
				mT__29(); 

				}
				break;
			case 3 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:22: T__30
				{
				mT__30(); 

				}
				break;
			case 4 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:28: T__31
				{
				mT__31(); 

				}
				break;
			case 5 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:34: T__32
				{
				mT__32(); 

				}
				break;
			case 6 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:40: T__33
				{
				mT__33(); 

				}
				break;
			case 7 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:46: T__34
				{
				mT__34(); 

				}
				break;
			case 8 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:52: T__35
				{
				mT__35(); 

				}
				break;
			case 9 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:58: T__36
				{
				mT__36(); 

				}
				break;
			case 10 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:64: T__37
				{
				mT__37(); 

				}
				break;
			case 11 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:70: T__38
				{
				mT__38(); 

				}
				break;
			case 12 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:76: T__39
				{
				mT__39(); 

				}
				break;
			case 13 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:82: T__40
				{
				mT__40(); 

				}
				break;
			case 14 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:88: T__41
				{
				mT__41(); 

				}
				break;
			case 15 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:94: T__42
				{
				mT__42(); 

				}
				break;
			case 16 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:100: T__43
				{
				mT__43(); 

				}
				break;
			case 17 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:106: T__44
				{
				mT__44(); 

				}
				break;
			case 18 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:112: T__45
				{
				mT__45(); 

				}
				break;
			case 19 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:118: T__46
				{
				mT__46(); 

				}
				break;
			case 20 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:124: T__47
				{
				mT__47(); 

				}
				break;
			case 21 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:130: T__48
				{
				mT__48(); 

				}
				break;
			case 22 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:136: T__49
				{
				mT__49(); 

				}
				break;
			case 23 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:142: T__50
				{
				mT__50(); 

				}
				break;
			case 24 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:148: T__51
				{
				mT__51(); 

				}
				break;
			case 25 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:154: T__52
				{
				mT__52(); 

				}
				break;
			case 26 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:160: T__53
				{
				mT__53(); 

				}
				break;
			case 27 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:166: T__54
				{
				mT__54(); 

				}
				break;
			case 28 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:172: T__55
				{
				mT__55(); 

				}
				break;
			case 29 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:178: T__56
				{
				mT__56(); 

				}
				break;
			case 30 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:184: T__57
				{
				mT__57(); 

				}
				break;
			case 31 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:190: T__58
				{
				mT__58(); 

				}
				break;
			case 32 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:196: T__59
				{
				mT__59(); 

				}
				break;
			case 33 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:202: T__60
				{
				mT__60(); 

				}
				break;
			case 34 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:208: T__61
				{
				mT__61(); 

				}
				break;
			case 35 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:214: T__62
				{
				mT__62(); 

				}
				break;
			case 36 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:220: T__63
				{
				mT__63(); 

				}
				break;
			case 37 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:226: T__64
				{
				mT__64(); 

				}
				break;
			case 38 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:232: T__65
				{
				mT__65(); 

				}
				break;
			case 39 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:238: T__66
				{
				mT__66(); 

				}
				break;
			case 40 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:244: T__67
				{
				mT__67(); 

				}
				break;
			case 41 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:250: T__68
				{
				mT__68(); 

				}
				break;
			case 42 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:256: T__69
				{
				mT__69(); 

				}
				break;
			case 43 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:262: T__70
				{
				mT__70(); 

				}
				break;
			case 44 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:268: T__71
				{
				mT__71(); 

				}
				break;
			case 45 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:274: T__72
				{
				mT__72(); 

				}
				break;
			case 46 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:280: T__73
				{
				mT__73(); 

				}
				break;
			case 47 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:286: T__74
				{
				mT__74(); 

				}
				break;
			case 48 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:292: T__75
				{
				mT__75(); 

				}
				break;
			case 49 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:298: T__76
				{
				mT__76(); 

				}
				break;
			case 50 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:304: T__77
				{
				mT__77(); 

				}
				break;
			case 51 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:310: T__78
				{
				mT__78(); 

				}
				break;
			case 52 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:316: T__79
				{
				mT__79(); 

				}
				break;
			case 53 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:322: T__80
				{
				mT__80(); 

				}
				break;
			case 54 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:328: T__81
				{
				mT__81(); 

				}
				break;
			case 55 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:334: T__82
				{
				mT__82(); 

				}
				break;
			case 56 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:340: T__83
				{
				mT__83(); 

				}
				break;
			case 57 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:346: T__84
				{
				mT__84(); 

				}
				break;
			case 58 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:352: T__85
				{
				mT__85(); 

				}
				break;
			case 59 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:358: T__86
				{
				mT__86(); 

				}
				break;
			case 60 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:364: T__87
				{
				mT__87(); 

				}
				break;
			case 61 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:370: T__88
				{
				mT__88(); 

				}
				break;
			case 62 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:376: T__89
				{
				mT__89(); 

				}
				break;
			case 63 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:382: T__90
				{
				mT__90(); 

				}
				break;
			case 64 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:388: BOOL_TYPE_DENOTER
				{
				mBOOL_TYPE_DENOTER(); 

				}
				break;
			case 65 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:406: INT_TYPE_DENOTER
				{
				mINT_TYPE_DENOTER(); 

				}
				break;
			case 66 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:423: FLOAT_TYPE_DENOTER
				{
				mFLOAT_TYPE_DENOTER(); 

				}
				break;
			case 67 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:442: BIN_LITERAL
				{
				mBIN_LITERAL(); 

				}
				break;
			case 68 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:454: OCT_LITERAL
				{
				mOCT_LITERAL(); 

				}
				break;
			case 69 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:466: DEC_LITERAL
				{
				mDEC_LITERAL(); 

				}
				break;
			case 70 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:478: HEX_LITERAL
				{
				mHEX_LITERAL(); 

				}
				break;
			case 71 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:490: NULL_LITERAL
				{
				mNULL_LITERAL(); 

				}
				break;
			case 72 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:503: THIS_LITERAL
				{
				mTHIS_LITERAL(); 

				}
				break;
			case 73 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:516: SUPER_LITERAL
				{
				mSUPER_LITERAL(); 

				}
				break;
			case 74 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:530: BOOL_LITERAL
				{
				mBOOL_LITERAL(); 

				}
				break;
			case 75 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:543: INT_LITERAL
				{
				mINT_LITERAL(); 

				}
				break;
			case 76 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:555: FLOAT_LITERAL
				{
				mFLOAT_LITERAL(); 

				}
				break;
			case 77 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:569: CHAR_LITERAL
				{
				mCHAR_LITERAL(); 

				}
				break;
			case 78 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:582: IDENT
				{
				mIDENT(); 

				}
				break;
			case 79 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:588: WS
				{
				mWS(); 

				}
				break;
			case 80 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:591: BLOCK_COMMENT
				{
				mBLOCK_COMMENT(); 

				}
				break;
			case 81 :
				// D:\\SoftwareEntwicklung\\C++\\HLC\\Tools\\XieXie-2.0\\repository\\docu\\Spec\\XieXie.g:1:605: LINE_COMMENT
				{
				mLINE_COMMENT(); 

				}
				break;

		}
	}


	protected DFA14 dfa14 = new DFA14(this);
	static final String DFA14_eotS =
		"\1\47\1\uffff\1\54\1\56\2\uffff\1\60\1\63\1\uffff\1\67\1\71\1\75\1\77"+
		"\1\uffff\1\102\1\uffff\1\105\2\uffff\1\107\14\51\1\uffff\1\141\2\uffff"+
		"\2\145\1\51\31\uffff\1\153\3\uffff\1\155\3\uffff\7\51\1\165\6\51\1\175"+
		"\3\51\1\u0081\5\51\7\uffff\1\145\2\51\4\uffff\1\u0089\6\51\1\uffff\5\51"+
		"\1\u0096\1\51\1\uffff\1\u0098\1\u0099\1\51\1\uffff\7\51\1\uffff\1\51\1"+
		"\u00a3\1\u00a4\3\51\1\u00a8\1\u00a9\4\51\1\uffff\1\51\2\uffff\1\u00b0"+
		"\5\51\1\u00b6\1\u00b7\1\u00b8\2\uffff\1\u00b9\2\51\2\uffff\1\51\1\u00bd"+
		"\1\u00be\2\51\1\u00b7\1\uffff\3\51\1\u00c4\1\u00c5\4\uffff\2\51\1\u00c8"+
		"\2\uffff\2\51\1\u00cb\1\u00cc\1\u00cd\2\uffff\1\51\1\u00cf\1\uffff\1\u00d0"+
		"\1\u00d1\3\uffff\1\u00d2\4\uffff";
	static final String DFA14_eofS =
		"\u00d3\uffff";
	static final String DFA14_minS =
		"\1\11\1\uffff\2\75\2\uffff\1\75\1\53\1\uffff\1\55\1\56\1\52\1\75\1\uffff"+
		"\1\74\1\uffff\1\75\2\uffff\1\75\1\156\1\157\1\141\1\145\1\154\1\141\1"+
		"\146\1\157\1\162\1\145\1\164\1\150\1\uffff\1\75\2\uffff\2\56\1\150\31"+
		"\uffff\1\75\3\uffff\1\75\3\uffff\1\144\1\145\1\157\1\163\1\141\1\156\1"+
		"\146\1\60\1\163\1\165\1\164\1\141\1\162\1\154\1\60\2\164\1\154\1\60\1"+
		"\164\1\141\1\151\1\160\1\151\7\uffff\1\56\1\151\1\165\4\uffff\1\60\1\141"+
		"\1\154\1\145\1\163\1\164\1\141\1\uffff\1\145\1\155\1\145\1\147\1\141\1"+
		"\60\1\163\1\uffff\2\60\1\154\1\uffff\1\165\2\164\1\145\1\154\1\163\1\145"+
		"\1\uffff\1\153\2\60\1\163\1\151\1\165\2\60\1\162\1\163\1\164\1\141\1\uffff"+
		"\1\145\2\uffff\1\60\1\162\1\151\1\143\1\162\1\145\3\60\2\uffff\1\60\1"+
		"\156\1\154\2\uffff\1\156\2\60\1\143\1\145\1\60\1\uffff\1\156\1\143\1\150"+
		"\2\60\4\uffff\1\165\1\164\1\60\2\uffff\1\150\1\162\3\60\2\uffff\1\145"+
		"\1\60\1\uffff\2\60\3\uffff\1\60\4\uffff";
	static final String DFA14_maxS =
		"\1\176\1\uffff\2\75\2\uffff\2\75\1\uffff\1\76\1\56\2\75\1\uffff\1\75\1"+
		"\uffff\1\76\2\uffff\1\75\1\156\1\162\2\157\1\170\1\157\1\156\1\165\1\162"+
		"\1\145\1\167\1\150\1\uffff\1\75\2\uffff\1\170\1\71\1\162\31\uffff\1\75"+
		"\3\uffff\1\75\3\uffff\1\144\1\145\1\157\1\163\1\141\1\156\1\146\1\172"+
		"\1\163\1\165\1\164\1\157\1\162\1\154\1\172\2\164\1\154\1\172\1\164\1\141"+
		"\1\151\1\160\1\151\7\uffff\1\71\1\151\1\165\4\uffff\1\172\1\141\1\154"+
		"\1\145\1\163\1\164\1\141\1\uffff\1\145\1\155\1\145\1\147\1\141\1\172\1"+
		"\163\1\uffff\2\172\1\154\1\uffff\1\165\2\164\1\145\1\154\1\163\1\145\1"+
		"\uffff\1\153\2\172\1\163\1\151\1\165\2\172\1\162\1\163\1\164\1\166\1\uffff"+
		"\1\145\2\uffff\1\172\1\162\1\151\1\143\1\162\1\145\3\172\2\uffff\1\172"+
		"\1\156\1\154\2\uffff\1\156\2\172\1\143\1\145\1\172\1\uffff\1\156\1\143"+
		"\1\150\2\172\4\uffff\1\165\1\164\1\172\2\uffff\1\150\1\162\3\172\2\uffff"+
		"\1\145\1\172\1\uffff\2\172\3\uffff\1\172\4\uffff";
	static final String DFA14_acceptS =
		"\1\uffff\1\1\2\uffff\1\6\1\7\2\uffff\1\15\4\uffff\1\30\1\uffff\1\35\1"+
		"\uffff\1\42\1\43\15\uffff\1\73\1\uffff\1\76\1\77\3\uffff\1\113\1\115\1"+
		"\116\1\117\1\3\1\2\1\5\1\4\1\11\1\10\1\13\1\14\1\12\1\17\1\20\1\21\1\16"+
		"\1\23\1\22\1\25\1\120\1\121\1\24\1\27\1\26\1\uffff\1\34\1\31\1\37\1\uffff"+
		"\1\36\1\45\1\44\30\uffff\1\75\1\74\1\103\1\104\1\106\1\105\1\114\3\uffff"+
		"\1\33\1\32\1\41\1\40\7\uffff\1\54\7\uffff\1\64\3\uffff\1\66\7\uffff\1"+
		"\46\14\uffff\1\61\1\uffff\1\101\1\65\11\uffff\1\100\1\50\3\uffff\1\55"+
		"\1\56\6\uffff\1\107\5\uffff\1\110\1\112\1\47\1\51\3\uffff\1\60\1\102\5"+
		"\uffff\1\111\1\72\2\uffff\1\57\2\uffff\1\67\1\70\1\71\1\uffff\1\53\1\62"+
		"\1\63\1\52";
	static final String DFA14_specialS =
		"\u00d3\uffff}>";
	static final String[] DFA14_transitionS = {
			"\2\52\1\uffff\2\52\22\uffff\1\52\1\1\3\uffff\1\2\1\3\1\50\1\4\1\5\1\6"+
			"\1\7\1\10\1\11\1\12\1\13\1\44\11\45\1\14\1\15\1\16\1\17\1\20\2\uffff"+
			"\32\51\1\21\1\uffff\1\22\1\23\2\uffff\1\24\1\25\1\26\1\27\1\30\1\31\2"+
			"\51\1\32\4\51\1\33\1\34\2\51\1\35\1\36\1\46\2\51\1\37\3\51\1\40\1\41"+
			"\1\42\1\43",
			"",
			"\1\53",
			"\1\55",
			"",
			"",
			"\1\57",
			"\1\61\21\uffff\1\62",
			"",
			"\1\64\17\uffff\1\65\1\66",
			"\1\70",
			"\1\73\4\uffff\1\74\15\uffff\1\72",
			"\1\76",
			"",
			"\1\100\1\101",
			"",
			"\1\103\1\104",
			"",
			"",
			"\1\106",
			"\1\110",
			"\1\112\2\uffff\1\111",
			"\1\113\12\uffff\1\114\2\uffff\1\115",
			"\1\116\11\uffff\1\117",
			"\1\120\1\uffff\1\121\11\uffff\1\122",
			"\1\125\12\uffff\1\123\2\uffff\1\124",
			"\1\126\7\uffff\1\127",
			"\1\130\5\uffff\1\131",
			"\1\132",
			"\1\133",
			"\1\134\1\136\1\uffff\1\135",
			"\1\137",
			"",
			"\1\140",
			"",
			"",
			"\1\146\63\uffff\1\142\14\uffff\1\143\10\uffff\1\144",
			"\1\146\1\uffff\12\147",
			"\1\150\11\uffff\1\151",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"\1\152",
			"",
			"",
			"",
			"\1\154",
			"",
			"",
			"",
			"\1\156",
			"\1\157",
			"\1\160",
			"\1\161",
			"\1\162",
			"\1\163",
			"\1\164",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\166",
			"\1\167",
			"\1\170",
			"\1\171\15\uffff\1\172",
			"\1\173",
			"\1\174",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\176",
			"\1\177",
			"\1\u0080",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u0082",
			"\1\u0083",
			"\1\u0084",
			"\1\u0085",
			"\1\u0086",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"\1\146\1\uffff\12\147",
			"\1\u0087",
			"\1\u0088",
			"",
			"",
			"",
			"",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u008a",
			"\1\u008b",
			"\1\u008c",
			"\1\u008d",
			"\1\u008e",
			"\1\u008f",
			"",
			"\1\u0090",
			"\1\u0091",
			"\1\u0092",
			"\1\u0093",
			"\1\u0094",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\4\51\1\u0095\25\51",
			"\1\u0097",
			"",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u009a",
			"",
			"\1\u009b",
			"\1\u009c",
			"\1\u009d",
			"\1\u009e",
			"\1\u009f",
			"\1\u00a0",
			"\1\u00a1",
			"",
			"\1\u00a2",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u00a5",
			"\1\u00a6",
			"\1\u00a7",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u00aa",
			"\1\u00ab",
			"\1\u00ac",
			"\1\u00ad\24\uffff\1\u00ae",
			"",
			"\1\u00af",
			"",
			"",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u00b1",
			"\1\u00b2",
			"\1\u00b3",
			"\1\u00b4",
			"\1\u00b5",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u00ba",
			"\1\u00bb",
			"",
			"",
			"\1\u00bc",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\1\u00bf",
			"\1\u00c0",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"\1\u00c1",
			"\1\u00c2",
			"\1\u00c3",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"",
			"",
			"",
			"\1\u00c6",
			"\1\u00c7",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"",
			"\1\u00c9",
			"\1\u00ca",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"",
			"\1\u00ce",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"",
			"",
			"\12\51\7\uffff\32\51\4\uffff\1\51\1\uffff\32\51",
			"",
			"",
			"",
			""
	};

	static final short[] DFA14_eot = DFA.unpackEncodedString(DFA14_eotS);
	static final short[] DFA14_eof = DFA.unpackEncodedString(DFA14_eofS);
	static final char[] DFA14_min = DFA.unpackEncodedStringToUnsignedChars(DFA14_minS);
	static final char[] DFA14_max = DFA.unpackEncodedStringToUnsignedChars(DFA14_maxS);
	static final short[] DFA14_accept = DFA.unpackEncodedString(DFA14_acceptS);
	static final short[] DFA14_special = DFA.unpackEncodedString(DFA14_specialS);
	static final short[][] DFA14_transition;

	static {
		int numStates = DFA14_transitionS.length;
		DFA14_transition = new short[numStates][];
		for (int i=0; i<numStates; i++) {
			DFA14_transition[i] = DFA.unpackEncodedString(DFA14_transitionS[i]);
		}
	}

	protected class DFA14 extends DFA {

		public DFA14(BaseRecognizer recognizer) {
			this.recognizer = recognizer;
			this.decisionNumber = 14;
			this.eot = DFA14_eot;
			this.eof = DFA14_eof;
			this.min = DFA14_min;
			this.max = DFA14_max;
			this.accept = DFA14_accept;
			this.special = DFA14_special;
			this.transition = DFA14_transition;
		}
		@Override
		public String getDescription() {
			return "1:1: Tokens : ( T__28 | T__29 | T__30 | T__31 | T__32 | T__33 | T__34 | T__35 | T__36 | T__37 | T__38 | T__39 | T__40 | T__41 | T__42 | T__43 | T__44 | T__45 | T__46 | T__47 | T__48 | T__49 | T__50 | T__51 | T__52 | T__53 | T__54 | T__55 | T__56 | T__57 | T__58 | T__59 | T__60 | T__61 | T__62 | T__63 | T__64 | T__65 | T__66 | T__67 | T__68 | T__69 | T__70 | T__71 | T__72 | T__73 | T__74 | T__75 | T__76 | T__77 | T__78 | T__79 | T__80 | T__81 | T__82 | T__83 | T__84 | T__85 | T__86 | T__87 | T__88 | T__89 | T__90 | BOOL_TYPE_DENOTER | INT_TYPE_DENOTER | FLOAT_TYPE_DENOTER | BIN_LITERAL | OCT_LITERAL | DEC_LITERAL | HEX_LITERAL | NULL_LITERAL | THIS_LITERAL | SUPER_LITERAL | BOOL_LITERAL | INT_LITERAL | FLOAT_LITERAL | CHAR_LITERAL | IDENT | WS | BLOCK_COMMENT | LINE_COMMENT );";
		}
	}

}
