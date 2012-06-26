import res_rc
from PyQt4.QtGui import QIcon, QSystemTrayIcon

class Indicator(QSystemTrayIcon):
    def __init__(self, parent=None):
        super(Indicator, self).__init__(parent)
        
        self.setIcon(QIcon('../res/img/SClogonobackground.png'))
        #self.setIcon(QIcon.fromTheme('face-smile'))

        ## Menu here
        # ...

        self.show()
