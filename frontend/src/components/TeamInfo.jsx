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

    const cardStyle = {
        flex: '0 0 calc(23% - 2em)',
        border: 'var(--border)',
        borderColor: 'var(--outline)',
        borderWidth: '3px',
        padding: '1em',
        borderRadius: '8px',
        backgroundColor: 'var(--pink)',
        color: 'var(--purple)',
    };

    // DISPLAY
    return (
        <div style={cardContainerStyle}>

            <div style={cardStyle}>
                <h2><FontAwesomeIcon icon={faHorseHead} />
                    <span>LOREM</span>
                    <FontAwesomeIcon icon={faHorseHead} />
                </h2>
                <p>Location: LOREM</p>
                <p>Conference: LOREM</p>
                <p>Division: LOREM</p>
            </div>

            <div style={{ width: '200px', display: 'flex', flexDirection: 'column' }}>
                <div className="row" style={{ gap: '0em' }}>
                    <FontAwesomeIcon icon={faHorseHead} style={{ padding: '0.5em', borderRadius: '60px', 
                        border: 'var(--border)',
                        borderTopColor: 'var(--pink)',
                        borderLeftColor: 'var(--pink)',}} />
                    <div style={{ width: '200px', marginTop: '-1em', borderTop: 'var(--border)' }}></div>
                    <FontAwesomeIcon icon={faHorseHead} style={{ padding: '0.5em', borderRadius: '50%', 
                        border: 'var(--border)',
                        borderTopColor: 'var(--pink)',
                        borderRightColor: 'var(--pink)',}} />
                </div>

                <div style={{ marginLeft: '.5em', marginRight: '.5em', 
                    borderRight: 'var(--border)', borderLeft: 'var(--border)' }}>
                        <h2>LOEM</h2>
                <p>Location: LOREM</p>
                <p>Conference: LOREM</p>
                <p>Division: LOREM</p>

                </div>

                <div className="row" style={{ gap: '0em' }}>
                    <FontAwesomeIcon icon={faHorseHead} style={{ padding: '0.5em', borderRadius: '50%', 
                        border: 'var(--border)',
                        borderBottomColor: 'var(--pink)',
                        borderLeftColor: 'var(--pink)',}} />
                    <div style={{ width: '200px', marginBottom: '-1em', borderTop: 'var(--border)' }}></div>
                    <FontAwesomeIcon icon={faHorseHead} style={{ padding: '0.5em', borderRadius: '50%', 
                        border: 'var(--border)',
                        borderBottomColor: 'var(--pink)',
                        borderRightColor: 'var(--pink)',}} />
                </div>
            </div>

            <div style={cardStyle}>
                <h2>LOEM</h2>
                <p>Location: LOREM</p>
                <p>Conference: LOREM</p>
                <p>Division: LOREM</p>
            </div> 

            <div style={cardStyle}>
                <h2>LOEM</h2>
                <p>Location: LOREM</p>
                <p>Conference: LOREM</p>
                <p>Division: LOREM</p>
            </div> 

            <div style={cardStyle}>
                <h2>LOEM</h2>
                <p>Location: LOREM</p>
                <p>Conference: LOREM</p>
                <p>Division: LOREM</p>
            </div> 

            <div style={cardStyle}>
                <h2>LOEM</h2>
                <p>Location: LOREM</p>
                <p>Conference: LOREM</p>
                <p>Division: LOREM</p>
            </div> 

        </div> // END row
    ); // END RETURN
}

export default TeamInfo;