document.addEventListener('DOMContentLoaded', function () {
    const feedbackBtn = document.getElementById('feedback-btn-link');
  
    const popup = document.createElement('div');
    popup.id = 'feedback-popup';
  
    // 获取版本号（如果有的话）
    const versionElement = document.querySelector('.shibuya-footer__version');
    const currentScript = document.currentScript || document.querySelector('script[src*="feedback.js"]');
    const version = docVersion;

    // 当前页面标题
    const pageTitle = document.title;
  
    popup.innerHTML = `
      <div style="margin-bottom: 10px; font-size: 14px;">
        ${LANG_JS.page}：<strong>${pageTitle}</strong><br>
        ${LANG_JS.version}：<strong>${version}</strong>
      </div>
      <textarea id="feedback-text" placeholder="${LANG_JS.feedbackPlaceholder}" rows="6"></textarea>
      <div style="margin-top: 20px; font-size: 13px;">
        ${LANG_JS.contactInfo}
      </div>
      <div class="flex-row" style="display: flex; gap: 20px; align-items: flex-start; margin-top: 8px;">
        <div style="display: flex; flex-direction: column; flex: 1; max-width: 200px;">
          <label for="feedback-name" style="margin-bottom: 4px;">${LANG_JS.nameLabel}</label>
          <input type="text" id="feedback-name" placeholder="${LANG_JS.namePlaceholder}" style="padding: 6px;" />
        </div>
        <div style="display: flex; flex-direction: column; flex: 1; max-width: 200px;">
          <label for="feedback-email" style="margin-bottom: 4px;">${LANG_JS.emailLabel}</label>
          <input type="email" id="feedback-email" placeholder="${LANG_JS.emailPlaceholder}" style="padding: 6px;" />
        </div>
      </div>
      <div class="feedback-actions">
        <button id="feedback-cancel">${LANG_JS.cancel}</button>
        <button id="feedback-submit">${LANG_JS.submit}</button>
      </div>
    `;

    document.body.appendChild(popup);
  
    feedbackBtn.onclick = (e) => {
      e.preventDefault();
      popup.style.display = 'block';
    };
  
    document.getElementById('feedback-cancel').onclick = () => popup.style.display = 'none';
    
    document.getElementById('feedback-submit').onclick = async () => {
      const submitBtn = document.getElementById('feedback-submit');
      submitBtn.disabled = true; // 禁用提交按钮
      submitBtn.textContent = LANG_JS.submitting; // 可选：显示提交中

      const name = document.getElementById('feedback-name').value.trim();
      const email = document.getElementById('feedback-email').value.trim();
      const text = document.getElementById('feedback-text').value.trim();
      if (!text) {
        alert(LANG_JS.emptyFeedback);
        submitBtn.disabled = false;
        submitBtn.textContent = LANG_JS.submit;        
        return;
      }
  
      // 如果填写了姓名或邮箱，则必须同时填写且邮箱格式正确
      if ((name && !email) || (!name && email)) {
          alert(LANG_JS.incompleteInfo);
          submitBtn.disabled = false;
          submitBtn.textContent = LANG_JS.submit;          
          return;
      }

      if (email) {
          const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
          if (!emailRegex.test(email)) {
              alert(LANG_JS.invalidEmail);
              submitBtn.disabled = false;
              submitBtn.textContent = LANG_JS.submit;              
              return;
          }
      }

      // 获取页面标题和版本号
      const pageTitle = document.title;
      const versionElement = document.querySelector('.shibuya-footer__version');
      const pageURL = window.location.href;

      // 组装请求数据
      const payload = {
          pageTitle,
          pageURL,
          version,
          feedback: text,
          name,
          email,
      };

      try {
        const response = await fetch('https://feedback.sifli.com/send_feedback', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(payload)
        });

        const result = await response.json();

        if (response.ok && result.success) {
            alert(LANG_JS.thankYou);
            // 清空输入框并关闭弹窗
            document.getElementById('feedback-text').value = '';
            document.getElementById('feedback-name').value = '';
            document.getElementById('feedback-email').value = '';
            popup.style.display = 'none';
        } else {
            alert(result.message || LANG_JS.submitFail);
        }
      } catch (error) {
          alert(LANG_JS.networkError);
      } finally {
        submitBtn.disabled = false;
        submitBtn.textContent = LANG_JS.submit;
      }
    };
  });
  