#include "pngwdgt.hpp"
#include "ui_pngwdgt.h"

template< typename T >
T
clamp( T const & p_A, T const & p_LOW, T const & p_HIGH ) {

	return  ( p_A < p_LOW ) ? p_LOW : ( p_HIGH < p_A ) ? p_HIGH : p_A;
}

PngWdgt::PngWdgt( QWidget * parent ) :
QWidget( parent ),
ui( new Ui::PngWdgt ),
arena2painter( A2B( -4., +4., 0., 400. ), A2B( -3., +3., 0., 300. ) ),
ball( 0,0 ),
ballV( 0.03, 0.01 ),
plyrLeft( -3., 0. ),
plyrRight( +3., 0. ),
scoreLeft( 0 ),
scoreRight( 0 ),
mlp( { 37 + 37, 7, 37 }, 1., 0., 1. ) {

	ui->setupUi( this );

	QObject::connect( & timer, SIGNAL( timeout( ) ), this, SLOT( slotTimerEvent( ) ) );

	timer.setInterval( static_cast< int >( 1000 / 60. ) );

	timer.start( );

	time.start( );
}

PngWdgt::~PngWdgt( ) {

	delete ui;
}

/*      0000 1111 2222 3333 4444 5555 6666
 * #### ####              #      #
 * #  #                   #      #
 * ####      ####         #    # #    #
 * #  #                        #      #
 * #  #                        #      #
 * ####           ####         #      #
 */

void
PngWdgt::drawSevenSegementDisplaySegment ( QPainter & p_painter, QRectF const & p_rect, unsigned char p_segment, QColor const & p_color ) {

	UV2ST
	display2arenaRect(
		A2B( 0, 4, p_rect.left( ), p_rect.right( ) ),
		A2B( 0, 6, p_rect.top( ), p_rect.bottom( ) ) );

	QRectF
	bar;

	switch ( p_segment ) {

		case 0 : {

			bar.setRect( 0, 0, 4, 1 );
			break;
		}

		case 1 : {

			bar.setRect( 0, 2, 4, 1 );
			break;
		}

		case 2 : {

			bar.setRect( 0, 5, 4, 1 );
			break;
		}

		case 3 : {

			bar.setRect( 3, 0, 1, 3 );
			break;
		}

		case 4 : {

			bar.setRect( 3, 2, 1, 4 );
			break;
		}

		case 5 : {

			bar.setRect( 0, 0, 1, 3 );
			break;
		}

		case 6 : {

			bar.setRect( 0, 2, 1, 4 );
			break;
		}

		default : {

			bar.setRect( 0, 0, 4, 6 );
			break;
		}
	}

	bar.setCoords(
		display2arenaRect.u2s.a2B( bar.left( ) ),
		display2arenaRect.v2t.a2B( bar.top( ) ),
		display2arenaRect.u2s.a2B( bar.right( ) ),
		display2arenaRect.v2t.a2B( bar.bottom( ) ) );

	bar.setCoords(
		arena2painter.u2s.a2B( bar.left( ) ),
		arena2painter.v2t.a2B( bar.top( ) ),
		arena2painter.u2s.a2B( bar.right( ) ),
		arena2painter.v2t.a2B( bar.bottom( ) ) );

	p_painter.fillRect( bar, p_color );
}

void
PngWdgt::drawSevenSegementDisplay ( QPainter & p_painter, QRectF const & p_rect, unsigned char p_digit, QColor const & p_color ) {

	switch ( p_digit ) {

		case 0 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 6, p_color );

			break;
		}

		case 1 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );

			break;
		}

		case 2 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 6, p_color );

			break;
		}

		case 3 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );

			break;
		}

		case 4 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );

			break;
		}

		case 5 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );

			break;
		}

		case 6 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 6, p_color );

			break;
		}

		case 7 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );

			break;
		}

		case 8 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 6, p_color );

			break;
		}

		case 9 : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );

			break;
		}

		default : {

			drawSevenSegementDisplaySegment( p_painter, p_rect, 0, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 1, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 2, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 3, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 4, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 5, p_color );
			drawSevenSegementDisplaySegment( p_painter, p_rect, 6, p_color );

			break;
		}
	}
}

void
PngWdgt::paintEvent( QPaintEvent * p_paintEvent ) {

	double
	racketHeight = 5.,
	pix = 1. / 30;

	int
	colOffR = .3 * ( time.elapsed( ) % 20 ),
	colOffG = .3 * ( time.elapsed( ) % 20 ),
	colOffB = .3 * ( time.elapsed( ) % 20 );

	QPainter
	painter( this );

	painter.fillRect(
		p_paintEvent->rect( ),
		QColor( 16, 18, 20 ) );

	painter.fillRect(
		arena2painter.u2s.a2B( -4. ),
		arena2painter.v2t.a2B( -3. ),
		arena2painter.u2s.dstA2B( -4., +4. ),
		arena2painter.v2t.dstA2B( -3., +3. ),
		QColor( colOffR, colOffG, colOffB ) );

	painter.fillRect(
		arena2painter.u2s.a2B( ball.x( ) - pix ),
		arena2painter.v2t.a2B( ball.y( ) - pix ),
		arena2painter.u2s.lenA2B( 2. * pix ),
		arena2painter.v2t.lenA2B( 2. * pix ),
		QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );

//	plyrLeft.ry( ) = -plyrRight.y( );

	ball.rx( ) += ballV.x( );
	ball.ry( ) += ballV.y( );

	if( ballV.x( ) > 0 ) {

		if( ( mlp.loop % 10000 ) == 0 ) {

			mlp.norm( );
		}

		QPointF
		ballCentre = ball - plyrRight;

		double
		ballTopY = ball.y( ) - plyrRight.y( ) - 15 * pix,
		ballBtmY = ball.y( ) - plyrRight.y( ) + 15 * pix;

		std::size_t
		alphaL = clamp( int( 36. * atan2( -ballCentre.x( ), ballTopY )        / ( M_PI ) ), 0, 36 ),
		alphaM = clamp( int( 36. * atan2( -ballCentre.x( ), ballCentre.y( ) ) / ( M_PI ) ), 0, 36 ),
		alphaR = clamp( int( 36. * atan2( -ballCentre.x( ), ballBtmY )        / ( M_PI ) ), 0, 36 );

		std::vector< double >
		angle( 37 + 37, 0. ),
		pos( 37, 0. );

		angle[ 37 + clamp< std::size_t >( 36 * ( plyrRight.y( ) + 3. ) / 6., 0, 36 ) ] = 1.;
		pos[ clamp< std::size_t >( 36 * ( plyrRight.y( ) + 3. ) / 6., 0, 36 ) ] = 1.;

		angle[ alphaL ] = 1.;
		angle[ alphaM ] = 1.;
		angle[ alphaR ] = 1.;

		for( std::size_t i = 0; i < angle.size( ); ++ i ) {

			painter.fillRect(
				arena2painter.u2s.a2B( .9 * ( -4. + 8. * i / ( angle.size( ) - 1 ) ) - pix ),
				arena2painter.v2t.a2B( +2.8 - pix ),
				arena2painter.u2s.lenA2B( 2. * pix ),
				arena2painter.v2t.lenA2B( 2. * pix ),
				QColor( int( 0xff * angle[ i ] ) & 0xff, int( 0xff * angle[ i ] ) & 0xff, int( 0xff * angle[ i ] ) & 0xff ) );
		}

		for( std::size_t i = 0; i < pos.size( ); ++ i ) {

			painter.fillRect(
				arena2painter.u2s.a2B( .9 * ( -4. + 8. * i / ( pos.size( ) - 1 ) ) - pix ),
				arena2painter.v2t.a2B( +2.4 - pix ),
				arena2painter.u2s.lenA2B( 2. * pix ),
				arena2painter.v2t.lenA2B( 2. * pix ),
				QColor( int( 0xff * pos[ i ] ) & 0xff, int( 0xff * pos[ i ] ) & 0xff, int( 0xff * pos[ i ] ) & 0xff ) );
		}
		mlp.remember( angle );
		mlp.teach( pos );
	}
	else {

		QPointF
		ballCentre = ball - plyrLeft;

		double
		ballTopY = ball.y( ) - plyrLeft.y( ) - 15 * pix,
		ballBtmY = ball.y( ) - plyrLeft.y( ) + 15 * pix;

		std::size_t
		alphaL = clamp( int( 36. * atan2( ballCentre.x( ), ballTopY )        / ( M_PI ) ), 0, 36 ),
		alphaM = clamp( int( 36. * atan2( ballCentre.x( ), ballCentre.y( ) ) / ( M_PI ) ), 0, 36 ),
		alphaR = clamp( int( 36. * atan2( ballCentre.x( ), ballBtmY )        / ( M_PI ) ), 0, 36 );

		std::vector< double >
		angle( 37 + 37, 0. );

		angle[ alphaL ] = 1.;
		angle[ alphaM ] = 1.;
		angle[ alphaR ] = 1.;
		angle[ 37 + clamp< std::size_t >( 36 * ( plyrLeft.y( ) + 3. ) / 6., 0, 36 ) ] = 1.;

		for( std::size_t i = 0; i < angle.size( ); ++ i ) {

			painter.fillRect(
				arena2painter.u2s.a2B( .9 * ( -4. + 8. * i / ( angle.size( ) - 1 ) ) - pix ),
				arena2painter.v2t.a2B( +2.8 - pix ),
				arena2painter.u2s.lenA2B( 2. * pix ),
				arena2painter.v2t.lenA2B( 2. * pix ),
				QColor( int( 0xff * angle[ i ] ) & 0xff, int( 0xff * angle[ i ] ) & 0xff, int( 0xff * angle[ i ] ) & 0xff ) );
		}

		mlp.remember( angle );

		double
		sum = 0,
		nrm = 0;

		for( std::size_t i = 0; i < 37; ++ i ) {

			sum += i * mlp.output( i );
			nrm += mlp.output( i );

			painter.fillRect(
				arena2painter.u2s.a2B( .9 * ( -4. + 8. * i / 36. ) - pix ),
				arena2painter.v2t.a2B( 2.4 - pix ),
				arena2painter.u2s.lenA2B( 2. * pix ),
				arena2painter.v2t.lenA2B( 2. * pix ),
				QColor( int( 0xff * mlp.output( i ) ) & 0xff, int( 0xff * mlp.output( i ) ) & 0xff, int( 0xff * mlp.output( i ) ) & 0xff ) );
		}
		std::cout << -3 + 6. * sum / ( 36. * nrm ) << std::endl;
		plyrLeft.ry( ) = -3 + 6. * sum / ( 36. * nrm );
	}

	if( ball.x( ) > 4. - pix  && 0. < ballV.x( ) ) {

		ballV = QPointF( -ballV.x( ), .05 * rand( ) / RAND_MAX - .025 );
		ball  = QPointF( 0., 4. * ( 1. * rand( ) / RAND_MAX - .5 ) );
		QSound::play( "../pong/sounds/goal.wav" );
		++ scoreRight;
	}

	if( ball.x( ) < -( 4. - pix ) && ballV.x( ) < 0. ) {

		ballV = QPointF( -ballV.x( ), .05 * rand( ) / RAND_MAX - .025 );
		ball  = QPointF( 0., 4. * ( 1. * rand( ) / RAND_MAX - .5 ) );
		QSound::play( "../pong/sounds/goal.wav" );
		++ scoreLeft;
	}

	if( ball.y( ) > 3. - pix ) {

		ballV.ry( ) = -ballV.y( );
		QSound::play( "../pong/sounds/wall.wav" );
	}

	if( ball.y( ) < -( 3. - pix ) ) {

		ballV.ry( ) = -ballV.y( );
		QSound::play( "../pong/sounds/wall.wav" );
	}

	if( 0. < ballV.x( ) &&
		ball.x( ) > plyrRight.x( ) - 2. * pix  &&
		ball.x( ) < plyrRight.x( ) &&
		ball.y( ) < plyrRight.y( ) + ( racketHeight + 1 ) * pix &&
		ball.y( ) > plyrRight.y( ) - ( racketHeight + 1 ) * pix ) {

		ballV = QPointF( -ballV.x( ), .2 * ( ball.y( ) - plyrRight.y( ) ) );
		QSound::play( "../pong/sounds/pad.wav" );
	}

	if( ballV.x( ) < 0. &&
		ball.x( ) < plyrLeft.x( ) + 2 * pix &&
		ball.x( ) > plyrLeft.x( ) &&
		ball.y( ) < plyrLeft.y( ) + ( racketHeight + 1 ) * pix &&
		ball.y( ) > plyrLeft.y( ) - ( racketHeight + 1 ) * pix ) {

		ballV = QPointF( -ballV.x( ), .2 * ( ball.y( ) - plyrLeft.y( ) ) );
		QSound::play( "../pong/sounds/pad.wav" );
	}

	painter.fillRect(
		arena2painter.u2s.a2B( plyrLeft.x( ) - pix ),
		arena2painter.v2t.a2B( plyrLeft.y( ) - racketHeight * pix ),
		arena2painter.u2s.lenA2B( 2. * pix ),
		arena2painter.v2t.lenA2B( 2. * racketHeight * pix ),
		QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );

	painter.fillRect(
		arena2painter.u2s.a2B( plyrRight.x( ) - pix ),
		arena2painter.v2t.a2B( plyrRight.y( ) - racketHeight * pix ),
		arena2painter.u2s.lenA2B( 2. * pix ),
		arena2painter.v2t.lenA2B( 2. * racketHeight * pix ),
		QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );

	for( int i = -12; i < 12; ++ i ) {

		painter.fillRect(
			arena2painter.u2s.a2B( - .5 * pix ),
			arena2painter.v2t.a2B( 1. / 12. + 3. / 12. * i ),
			arena2painter.u2s.lenA2B( pix ),
			arena2painter.v2t.lenA2B( 1. / 12. ),
			QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
	}

	drawSevenSegementDisplay( painter, QRectF( -1., -2.7, .2, .3 ), ( scoreRight / 100 ), QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
	drawSevenSegementDisplay( painter, QRectF( -.7, -2.7, .2, .3 ), ( scoreRight / 10 ) % 10, QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
	drawSevenSegementDisplay( painter, QRectF( -.4, -2.7, .2, .3 ), scoreRight  % 10, QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
	drawSevenSegementDisplay( painter, QRectF( +.2, -2.7, .2, .3 ), ( scoreLeft / 100 ), QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
	drawSevenSegementDisplay( painter, QRectF( +.5, -2.7, .2, .3 ), ( scoreLeft / 10 ) % 10, QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
	drawSevenSegementDisplay( painter, QRectF( +.8, -2.7, .2, .3 ), scoreLeft % 10, QColor( 200 + 10 * colOffR, 200 + 10 * colOffG, 200 + 10 * colOffB ) );
}

void
PngWdgt::mouseMoveEvent( QMouseEvent * p_mouseEvent ) {

	plyrRight.ry( ) = arena2painter.v2t.b2A( p_mouseEvent->y( ) );
}

void
PngWdgt::resizeEvent( QResizeEvent * p_resizeEvent ) {

	double
	mx  = .5 * p_resizeEvent->size( ).width( ),
	my  = .5 * p_resizeEvent->size( ).height( ),
	dx = mx,
	dy = my;

	if( p_resizeEvent->size( ).width( ) < 4. / 3. * p_resizeEvent->size( ).height( ) ) {

		dy = mx * 3. / 4.;
	}
	else {

		dx = my * 4. / 3.;
	};

	arena2painter.u2s.resetB( mx - dx, mx + dx );
	arena2painter.v2t.resetB( my - dy, my + dy );
}

void
PngWdgt::keyReleaseEvent( QKeyEvent * p_keyEvent ) {

	if( p_keyEvent->key() == Qt::Key_Escape || p_keyEvent->key() == Qt::Key_Q )

		this->close( );

	if( p_keyEvent->key() == Qt::Key_F )

		this->setWindowState( windowState( ) ^ Qt::WindowFullScreen );
}

void
PngWdgt::slotTimerEvent( ) {

	update( );
}