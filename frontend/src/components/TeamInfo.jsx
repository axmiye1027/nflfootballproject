// Displaying basic team information card
// Will modify later to accept props for dynamic data
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faHorseHead } from '@fortawesome/free-solid-svg-icons'
    
function TeamInfo() {

    // STYLE
    const cardContainerStyle = {
        display: 'flex',
        flexWrap: 'wrap',
        gap: '1em',
        justifyContent: 'center',
        alignItems: 'stretch',
        padding: '2em',
        width: '100%',
        boxSizing: 'border-box',
    };

    // card lengths
    const cardWidth = {
        flex: '0 0 calc(23% - 2em)',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
    };

    // horse icon
    const iconStyle = {
        padding: '0.5em',
        borderRadius: '50%', 
        border: 'var(--border)',
    };

    // border between icons
    const dividerStyle = {
        width: '100%',
        borderTop: 'var(--border)',
    };

    // info card container
    const infoCardStyle = {
        width: 'calc(100% - 4em)',
        padding: '1em',
        marginLeft: '.5em',
        marginRight: '.5em', 
        borderRight: 'var(--border)',
        borderLeft: 'var(--border)',
    };

    // DISPLAY
    return (
        <div style={cardContainerStyle}>

            { /* CARD */}
            <div style={cardWidth}>
                { /* TOP */}
                <div className="row" style={{ gap: '0em' }}>
                    <FontAwesomeIcon icon={faHorseHead} className="bt-0 bl-0" style={iconStyle} />
                    <div style={dividerStyle}></div>
                    <FontAwesomeIcon icon={faHorseHead} className="bt-0 br-0" style={iconStyle} />
                </div>

                { /* MIDDLE - INFO CARD */}
                <div style={infoCardStyle}>
                    <h2>LOEM</h2>
                    <p>Location: LOREM</p>
                    <p>Conference: LOREM</p>
                    <p>Division: LOREM</p>

                </div>

                { /* BOTTOM */}
                <div className="row" style={{ gap: '0em' }}>
                    <FontAwesomeIcon icon={faHorseHead} className="bb-0 bl-0" style={iconStyle} />
                    <div style={dividerStyle}></div>
                    <FontAwesomeIcon icon={faHorseHead} className="bb-0 br-0" style={iconStyle} />
                </div>
            </div>
            { /* END CARD */}

        </div> // END row
    ); // END RETURN
}

export default TeamInfo;