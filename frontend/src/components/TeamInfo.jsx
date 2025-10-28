// Displaying basic team information card
// Will modify later to accept props for dynamic data
    
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
        padding: '1em',
        borderRadius: '8px',
    };

    // DISPLAY
    return (
        <div style={cardContainerStyle}>

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